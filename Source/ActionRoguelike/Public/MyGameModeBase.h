// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Engine/DataTable.h"
#include "GAS/SAction.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
class UEnvQuery;
class USSaveGame;
class UDataTable;
class USMonsterData;

USTRUCT(BlueprintType)
struct FMonsterInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMonsterInfo();
	
	/*�޸�Ϊ�첽��ȡ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MonsterData")
	USMonsterData* MonsterData;*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MonsterData")
	FPrimaryAssetId PrimaryAssetId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster Info")
	float Weight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster Info")
	float SpawnCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster Info")
	float KillReward;


};




UCLASS()
class ACTIONROGUELIKE_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	//Monster ���ݱ��
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="DataTable")
	UDataTable* MonsterInfo;

	UPROPERTY()
	FTimerHandle TimeHandle_SpawnRandom;

	//EQS
	UPROPERTY(EditAnywhere,Category = "AI")
	UEnvQuery* EnvQuery_SpawnRandom;

	UPROPERTY(EditAnywhere,Category="AI")
	float SpawnDelayTime; //���������ɵ��ˣ�Ĭ�� = 2

	UPROPERTY(EditAnywhere,Category="AI")
	UCurveFloat* DifficultyCurve; //�Ѷ����ߣ��������AI���趨���ߣ�Ĭ��=10

	//����AI
	UFUNCTION()
	void SpawnAI(); 

	//����
	UPROPERTY(EditAnywhere, Category = "Respawn")
		float	RespawnTime; //�����ȴ�ʱ��
	UFUNCTION()
		void RespawnPlayer(AController* NewPc); //����
	
	
	//����_ɱ�����е���
	UFUNCTION(Exec)
	void KillAll();

	//�󶨲�ѯ��ɺ���
	UFUNCTION()
	void OnQueryFinishedEvent(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	//�첽������Դ��ί�е��ú���
	UFUNCTION()
	void OnMosterDataAssetLoad(FPrimaryAssetId MonsterPrimaryAssetId, FVector SpawnLocation);

	//��������
	UPROPERTY(EditAnywhere,Category="Credits")
	TArray< TSubclassOf<AActor> > PowerClass;
	//ɱ�з���
	UPROPERTY(EditAnywhere,Category="Credits")
	int32 KillCredits;
	//���ɵĵ�������
	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 DesiredPowerCount;
	//���ɵ��ߵļ��
	UPROPERTY(EditAnywhere, Category = "Credits")
	float DesiredPowerDistence;
	
	UPROPERTY(EditAnywhere, Category = "Credits")
	UEnvQuery* EQS_FindPowerLocation;

	UFUNCTION()
	void OnQueryPowerLocation(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	//�����ļ��������
	UPROPERTY()
	FString SaveSlotName;

	UPROPERTY(BlueprintReadOnly,Category="SaveGame")
	USSaveGame* CurrentSaveGame;



public:
	
	AMyGameModeBase();

	virtual void StartPlay() override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	UFUNCTION()
	void OnActorKiller(AActor* killed, AActor* Instigat);
	
	//-----������Ϸ
	UFUNCTION(BlueprintCallable,Category ="SaveGame" )
	void WriteSaveGame();

	UFUNCTION()
	void LoadSaveGame();
};
