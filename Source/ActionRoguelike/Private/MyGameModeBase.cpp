// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EngineUtils.h"
#include "AI/SAICharacter.h"
#include "SCharacter.h"
#include "SPlayerState.h"


static TAutoConsoleVariable<bool> CvarSpawnBots(TEXT("su.spawnBots"),true,TEXT("enable spawnBots"),ECVF_Cheat);
AMyGameModeBase::AMyGameModeBase()
{
	SpawnDelayTime = 2;
	KillCredits = 20;
	DesiredPowerCount = 5;
	DesiredPowerDistence = 1000;
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

void AMyGameModeBase::OnQueryPowerLocation(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus) {
	//
	if (!EEnvQueryStatus::Success) {
		return;
	}
	TArray<FVector>  Locations = QueryInstance->GetResultsAsLocations();

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

	if (Locations.IsValidIndex(0)) {
		GetWorld()->SpawnActor<AActor>(SpawnAIClass,Locations[0],FRotator::ZeroRotator);
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

