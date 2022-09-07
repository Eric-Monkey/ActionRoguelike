// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
class UEnvQuery;

UCLASS()
class ACTIONROGUELIKE_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	FTimerHandle TimeHandle_SpawnRandom;

	//EQS
	UPROPERTY(EditAnywhere,Category = "AI")
	UEnvQuery* EnvQuery_SpawnRandom;

	UPROPERTY(EditAnywhere,Category="AI")
	float SpawnDelayTime; //���������ɵ��ˣ�Ĭ�� = 2

	UPROPERTY(EditAnywhere,Category="AI")
	UCurveFloat* DifficultyCurve; //�Ѷ����ߣ��������AI���趨���ߣ�Ĭ��=10

	UPROPERTY(EditAnywhere,Category="AI")
	TSubclassOf<AActor> SpawnAIClass; //���ɵ���

	UFUNCTION()
	void SpawnAI();

	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION()
	void OnQueryFinishedEvent(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	//����
	UPROPERTY(EditAnywhere)
	float	RespawnTime;
	UFUNCTION()
	void RespawnPlayer(AController* NewPc);
public:
	UFUNCTION()
	void OnActorKiller(AActor* killed, AActor* Instigat);

public:

	AMyGameModeBase();

	virtual void StartPlay() override;
};
