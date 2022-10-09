// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EngineUtils.h"
#include "AI/SAICharacter.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/SSaveGame.h"
#include "GameFramework/GameStateBase.h"
#include "SInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "SMonsterData.h"
#include "SAttributeComponent.h"
#include "GAS/SActionComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"


static TAutoConsoleVariable<bool> CvarSpawnBots(TEXT("su.spawnBots"),false ,TEXT("enable spawnBots"),ECVF_Cheat);
AMyGameModeBase::AMyGameModeBase()
{
	SpawnDelayTime = 2;
	KillCredits = 20;
	DesiredPowerCount = 5;
	DesiredPowerDistence = 1000;

	SaveSlotName = "MySaveGame_01";
}


FMonsterInfo::FMonsterInfo()
{
	Weight = 1.f;
	SpawnCost = 10.f;
	KillReward = 5.f;
}


void AMyGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	UE_LOG(LogTemp,Log,TEXT("gamemod start"));
	GetWorld()->GetTimerManager().SetTimer(TimeHandle_SpawnRandom ,this, &AMyGameModeBase::SpawnAI ,SpawnDelayTime,true);

	//刷道具列表需要有道具才会刷
	if (PowerClass.Num()>0) {
		
		UEnvQueryInstanceBlueprintWrapper* EnvQueryInstance = UEnvQueryManager::RunEQSQuery(this, EQS_FindPowerLocation, this, EEnvQueryRunMode::AllMatching, nullptr);
		if (ensure(EnvQueryInstance)) {
			EnvQueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AMyGameModeBase::OnQueryPowerLocation);
		}

	}
}

void AMyGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	//初始化SaveGame
	LoadSaveGame();
}

void AMyGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	
	ASPlayerState* Ps = NewPlayer->GetPlayerState<ASPlayerState>();
	//
	if (Ps) {
		Ps->GetSaveGameData(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}


void AMyGameModeBase::OnQueryPowerLocation(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus) {
	//
	if (!EEnvQueryStatus::Success) {
		return;
	}
	TArray<FVector>  Locations = QueryInstance->GetResultsAsLocations();
	if (! Locations.IsValidIndex(0)) {

		UE_LOG(LogTemp, Warning, TEXT("OnQueryPowerLocation Fail: Please Cheak (NavMeshBoundsVolume Woark normal or EQS Running ok? )"))
		return;
	}
	

	TArray<FVector> UsedLoactions;
	//当前道具数量<期望道具数量 就刷道具
	int32 CurPowerCount=0;
	while (CurPowerCount<DesiredPowerCount) {
		//随意获取位置
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector RandomLocation = Locations[RandomLocationIndex];

		
		//判断选取位置是否合法
		bool isValidLocation=false;

		//已加入位置为空，则直接加入
		if (0 == UsedLoactions.Num()) {
			isValidLocation = true;
		}

		for (FVector UsedLoaction :UsedLoactions) {
			float Distence = (RandomLocation-UsedLoaction).Size();			
			if (Distence > DesiredPowerDistence) {
				isValidLocation = true;		//标记合法退出循环，而不是没找到退出循环
				break;
			}
		}
		//合法
		if (isValidLocation) {
			UsedLoactions.Add(RandomLocation);		//加入usedlocations,
			Locations.RemoveAt(RandomLocationIndex);		//从locations移除避免随机重复选取
			int32 RandomPowerClassIndex=FMath::RandRange(0, PowerClass.Num()-1);		//获取生成道具种类下标
			TSubclassOf<AActor> RandomPowerClass = PowerClass[RandomPowerClassIndex];		//获取道具种类
			
			GetWorld()->SpawnActor<AActor>(RandomPowerClass, RandomLocation, FRotator::ZeroRotator);		//刷道具
			++CurPowerCount;
		}
		//选取位置不合法
		if (!isValidLocation) {
			continue;//选取下一个点
		}

	}

}

void AMyGameModeBase::SpawnAI()
{
	//RunEqs
	UEnvQueryInstanceBlueprintWrapper* EnvQueryInstance = UEnvQueryManager::RunEQSQuery(this, EnvQuery_SpawnRandom , this , EEnvQueryRunMode::RandomBest5Pct , nullptr);
	if ( ensure(EnvQueryInstance) )
	{
		EnvQueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AMyGameModeBase::OnQueryFinishedEvent);
	}
}

void AMyGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {
		ASAICharacter* AI = *It;
		if (AI->IsAlive()) {
			USAttributeComponent::GetAttribute(AI)->Kill(this);
		}
	}
}

void AMyGameModeBase::OnQueryFinishedEvent(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	//控制spawnAI
	if (!CvarSpawnBots.GetValueOnGameThread()) {return; }

	//查看世界总AL
	int AliveAI = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {
		ASAICharacter* AI = *It;
		if (AI->IsAlive()) {
			++AliveAI ;
		}
	}
	
	int MaxAI = 10;
	if (DifficultyCurve) {
		MaxAI = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}

	if (AliveAI>=MaxAI) {
		return;
	}

	TArray<FVector>  Locations = QueryInstance->GetResultsAsLocations();
	if (QueryStatus != EEnvQueryStatus::Success) {
		return;
	}

	//读取 Monster 数据表格的所有数据行
	TArray<FMonsterInfo*> MonsterInfos;
	MonsterInfo->GetAllRows<FMonsterInfo>("", MonsterInfos);

	//从 Monster 数据表随意读取一行数据
	if (MonsterInfos.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("please cheak the DataTable is null ? Read MonsterDataTable to Spawn AI fail"));
		return;
	}
	int32 MonsterInfosRandomIndex = FMath::RandRange(0,MonsterInfos.Num()-1);
	FMonsterInfo* SelectRowMonsterInfo =  MonsterInfos[MonsterInfosRandomIndex];

	
	// 查询位置 不合法
	if (! Locations.IsValidIndex(0)) {
		return;
	}

	//异步加载生成 Monster 的信息
	UAssetManager* AssetManager = UAssetManager::GetIfValid();
	if (AssetManager) {
		TArray<FName> LoadBundles;
		FStreamableDelegate StreamableDelegate = FStreamableDelegate::CreateUObject(this, &AMyGameModeBase::OnMosterDataAssetLoad ,SelectRowMonsterInfo->PrimaryAssetId , Locations[0]);
		AssetManager->LoadPrimaryAsset(SelectRowMonsterInfo->PrimaryAssetId,LoadBundles,StreamableDelegate);
	}

}

//加载完资源后生成AI
void AMyGameModeBase::OnMosterDataAssetLoad(FPrimaryAssetId MonsterPrimaryAssetId, FVector SpawnLocation)
{
	
	UAssetManager* AssetManager = UAssetManager::GetIfValid();
	if (AssetManager) {
		//数据已经读入AssetManager,从AssetManager读取需要的数据
		USMonsterData* MonsterData = Cast<USMonsterData>(AssetManager->GetPrimaryAssetObject(MonsterPrimaryAssetId)); 
		if (MonsterData) {
			
			//生成AI 
			AActor* NewActor = GetWorld()->SpawnActor<AActor>(MonsterData->SpawnAIClass, SpawnLocation , FRotator::ZeroRotator);

			//给新生成AI，初始化Action
			if (NewActor) {
				USActionComponent* ActionComp = Cast< USActionComponent >(NewActor->GetComponentByClass(USActionComponent::StaticClass()));

				if (ActionComp) {
					for (TSubclassOf<USAction> Action : MonsterData->Actions) {
						ActionComp->AddAction(NewActor, Action);
					}
				}
			}
		}
	}
	

}


void AMyGameModeBase::RespawnPlayer(AController* NewPc)
{
	if (ensure(NewPc)) {
		NewPc->UnPossess();// 这样会导致 1、人物没有被销毁 2、因为UI没有写在PC,Pawn脱离后Ui失效 

		RestartPlayer(NewPc); //如果 PC 的 Pawn 不为空，则该函数将人物移动到起始位置，所以unpossesed
	}
}

void AMyGameModeBase::OnActorKiller(AActor* killed, AActor* Instigat)
{
	ASCharacter* Play = Cast<ASCharacter>(killed);//判断被打死 Actor 是不是 player
	if (Play) {
		FTimerHandle TimeHandle_Respawn;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"RespawnPlayer",Play->GetController());

		RespawnTime = 2.f;
		GetWorldTimerManager().SetTimer(TimeHandle_Respawn,Delegate,RespawnTime,false);
	}

	//如果Ai被打死
	ASAICharacter*  AIPlayer= Cast<ASAICharacter>(killed);
	APawn* MyPawn = Cast<APawn>(Instigat);
	if (AIPlayer && MyPawn) {
		ASPlayerState* PS= MyPawn->GetPlayerState<ASPlayerState>();
		if (PS) {
			PS->AddCredits(KillCredits);
		} 
	}
}



void AMyGameModeBase::WriteSaveGame() {

	for (int i = 0;i < GameState->PlayerArray.Num();i++) {

		ASPlayerState* PS = Cast<ASPlayerState>(GameState->PlayerArray[i]);

		PS->SetSaveGameData(CurrentSaveGame);
		break; //单人模式所以直接跳出了
	}
	
	//every Save to clear ActorDatas
	CurrentSaveGame->ActorDatas.Empty();
	
	//Save Moving Actor
	for (FActorIterator It(GetWorld());It ; ++It) {
		AActor* Actor= *It;
		if (Actor && Actor->Implements<USInterface>()) {
			
			FActorData ActorData;
			ActorData.ActorName = Actor->GetName();
			ActorData.ActorTransform = Actor->GetActorTransform();
			

			//Actor自身写入SaveGame的字节数组
			FMemoryWriter MemWrite(ActorData.BinActorData);
			FObjectAndNameAsStringProxyArchive Ar(MemWrite,true);
			Ar.ArIsSaveGame = true;
			Actor->Serialize(Ar);			
			
			CurrentSaveGame->ActorDatas.Add(ActorData);
		}
		
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame,SaveSlotName,0);

}

void AMyGameModeBase::LoadSaveGame() {

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName,0)) {

		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
		if (CurrentSaveGame==nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Fail to load SaveGame Data"));
			return;
		}


		for (FActorIterator It(GetWorld()); It; ++It) {
			AActor* Actor = *It;
			//只关注实现接口的Actor
			if (!Actor && !Actor->Implements<USInterface>()) {
				continue;
			}
			for ( FActorData ActorData :CurrentSaveGame->ActorDatas)
			{
				if (ActorData.ActorName == Actor->GetName()) {
					//设置Actor的变换
					Actor->SetActorRelativeTransform(ActorData.ActorTransform);

					//设置Actor变量
					FMemoryReader MemRead(ActorData.BinActorData);
					FObjectAndNameAsStringProxyArchive Ar(MemRead,true);
					Ar.ArIsSaveGame = true;
					Actor->Serialize(Ar);

					ISInterface::Execute_OnActorLoad(Actor);
					break;
				}

			}

		}

		UE_LOG(LogTemp, Log, TEXT("Successed to load SaveGame Data"));
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>( UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()) );
		UE_LOG(LogTemp, Log, TEXT("Create SaveGame Data"));
	}

}
