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

	//ˢ�����б���Ҫ�е��߲Ż�ˢ
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

