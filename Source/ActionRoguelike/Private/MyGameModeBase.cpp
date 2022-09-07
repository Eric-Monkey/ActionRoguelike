// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EngineUtils.h"
#include "AI/SAICharacter.h"
#include "SCharacter.h"


static TAutoConsoleVariable<bool> CvarSpawnBots(TEXT("su.spawnBots"),true,TEXT("enable spawnBots"),ECVF_Cheat);
AMyGameModeBase::AMyGameModeBase()
{
	SpawnDelayTime = 2;
}

void AMyGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorld()->GetTimerManager().SetTimer(TimeHandle_SpawnRandom ,this, &AMyGameModeBase::SpawnAI ,SpawnDelayTime,true);
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

	if (Locations.IsValidIndex(0)) {
		GetWorld()->SpawnActor<AActor>(SpawnAIClass,Locations[0],FRotator::ZeroRotator);
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
	ASCharacter* Play = Cast<ASCharacter>(killed);//�жϱ����� Actor�ǲ��� player
	if (Play) {
		FTimerHandle TimeHandle_Respawn;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"RespawnPlayer",Play->GetController());

		RespawnTime = 2.f;
		GetWorldTimerManager().SetTimer(TimeHandle_Respawn,Delegate,RespawnTime,false);
	}
}

