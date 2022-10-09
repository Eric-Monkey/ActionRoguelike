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

	//ˢ�����б���Ҫ�е��߲Ż�ˢ
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
	//��ʼ��SaveGame
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
	//��ǰ��������<������������ ��ˢ����
	int32 CurPowerCount=0;
	while (CurPowerCount<DesiredPowerCount) {
		//�����ȡλ��
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector RandomLocation = Locations[RandomLocationIndex];

		
		//�ж�ѡȡλ���Ƿ�Ϸ�
		bool isValidLocation=false;

		//�Ѽ���λ��Ϊ�գ���ֱ�Ӽ���
		if (0 == UsedLoactions.Num()) {
			isValidLocation = true;
		}

		for (FVector UsedLoaction :UsedLoactions) {
			float Distence = (RandomLocation-UsedLoaction).Size();			
			if (Distence > DesiredPowerDistence) {
				isValidLocation = true;		//��ǺϷ��˳�ѭ����������û�ҵ��˳�ѭ��
				break;
			}
		}
		//�Ϸ�
		if (isValidLocation) {
			UsedLoactions.Add(RandomLocation);		//����usedlocations,
			Locations.RemoveAt(RandomLocationIndex);		//��locations�Ƴ���������ظ�ѡȡ
			int32 RandomPowerClassIndex=FMath::RandRange(0, PowerClass.Num()-1);		//��ȡ���ɵ��������±�
			TSubclassOf<AActor> RandomPowerClass = PowerClass[RandomPowerClassIndex];		//��ȡ��������
			
			GetWorld()->SpawnActor<AActor>(RandomPowerClass, RandomLocation, FRotator::ZeroRotator);		//ˢ����
			++CurPowerCount;
		}
		//ѡȡλ�ò��Ϸ�
		if (!isValidLocation) {
			continue;//ѡȡ��һ����
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
	//����spawnAI
	if (!CvarSpawnBots.GetValueOnGameThread()) {return; }

	//�鿴������AL
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

	//��ȡ Monster ���ݱ�������������
	TArray<FMonsterInfo*> MonsterInfos;
	MonsterInfo->GetAllRows<FMonsterInfo>("", MonsterInfos);

	//�� Monster ���ݱ������ȡһ������
	if (MonsterInfos.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("please cheak the DataTable is null ? Read MonsterDataTable to Spawn AI fail"));
		return;
	}
	int32 MonsterInfosRandomIndex = FMath::RandRange(0,MonsterInfos.Num()-1);
	FMonsterInfo* SelectRowMonsterInfo =  MonsterInfos[MonsterInfosRandomIndex];

	
	// ��ѯλ�� ���Ϸ�
	if (! Locations.IsValidIndex(0)) {
		return;
	}

	//�첽�������� Monster ����Ϣ
	UAssetManager* AssetManager = UAssetManager::GetIfValid();
	if (AssetManager) {
		TArray<FName> LoadBundles;
		FStreamableDelegate StreamableDelegate = FStreamableDelegate::CreateUObject(this, &AMyGameModeBase::OnMosterDataAssetLoad ,SelectRowMonsterInfo->PrimaryAssetId , Locations[0]);
		AssetManager->LoadPrimaryAsset(SelectRowMonsterInfo->PrimaryAssetId,LoadBundles,StreamableDelegate);
	}

}

//��������Դ������AI
void AMyGameModeBase::OnMosterDataAssetLoad(FPrimaryAssetId MonsterPrimaryAssetId, FVector SpawnLocation)
{
	
	UAssetManager* AssetManager = UAssetManager::GetIfValid();
	if (AssetManager) {
		//�����Ѿ�����AssetManager,��AssetManager��ȡ��Ҫ������
		USMonsterData* MonsterData = Cast<USMonsterData>(AssetManager->GetPrimaryAssetObject(MonsterPrimaryAssetId)); 
		if (MonsterData) {
			
			//����AI 
			AActor* NewActor = GetWorld()->SpawnActor<AActor>(MonsterData->SpawnAIClass, SpawnLocation , FRotator::ZeroRotator);

			//��������AI����ʼ��Action
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
		NewPc->UnPossess();// �����ᵼ�� 1������û�б����� 2����ΪUIû��д��PC,Pawn�����UiʧЧ 

		RestartPlayer(NewPc); //��� PC �� Pawn ��Ϊ�գ���ú����������ƶ�����ʼλ�ã�����unpossesed
	}
}

void AMyGameModeBase::OnActorKiller(AActor* killed, AActor* Instigat)
{
	ASCharacter* Play = Cast<ASCharacter>(killed);//�жϱ����� Actor �ǲ��� player
	if (Play) {
		FTimerHandle TimeHandle_Respawn;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"RespawnPlayer",Play->GetController());

		RespawnTime = 2.f;
		GetWorldTimerManager().SetTimer(TimeHandle_Respawn,Delegate,RespawnTime,false);
	}

	//���Ai������
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
		break; //����ģʽ����ֱ��������
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
			

			//Actor����д��SaveGame���ֽ�����
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
			//ֻ��עʵ�ֽӿڵ�Actor
			if (!Actor && !Actor->Implements<USInterface>()) {
				continue;
			}
			for ( FActorData ActorData :CurrentSaveGame->ActorDatas)
			{
				if (ActorData.ActorName == Actor->GetName()) {
					//����Actor�ı任
					Actor->SetActorRelativeTransform(ActorData.ActorTransform);

					//����Actor����
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
