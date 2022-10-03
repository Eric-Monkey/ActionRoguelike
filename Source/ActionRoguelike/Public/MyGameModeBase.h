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
class USSaveGame;

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
	float SpawnDelayTime; //多少秒生成敌人，默认 = 2

	UPROPERTY(EditAnywhere,Category="AI")
	UCurveFloat* DifficultyCurve; //难度曲线，世界最大AI数设定曲线，默认=10

	UPROPERTY(EditAnywhere,Category="AI")
	TSubclassOf<AActor> SpawnAIClass; //生成的类
	//生成AI
	UFUNCTION()
	void SpawnAI(); 

	//重生
	UPROPERTY(EditAnywhere, Category = "Respawn")
		float	RespawnTime; //重生等待时间
	UFUNCTION()
		void RespawnPlayer(AController* NewPc); //重生
	
	
	//命令_杀死所有敌人
	UFUNCTION(Exec)
	void KillAll();

	//绑定查询完成函数
	UFUNCTION()
	void OnQueryFinishedEvent(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);


	//道具种类
	UPROPERTY(EditAnywhere,Category="Credits")
	TArray< TSubclassOf<AActor> > PowerClass;
	//杀敌分数
	UPROPERTY(EditAnywhere,Category="Credits")
	int32 KillCredits;
	//生成的道具数量
	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 DesiredPowerCount;
	//生成道具的间隔
	UPROPERTY(EditAnywhere, Category = "Credits")
	float DesiredPowerDistence;
	
	UPROPERTY(EditAnywhere, Category = "Credits")
	UEnvQuery* EQS_FindPowerLocation;

	UFUNCTION()
	void OnQueryPowerLocation(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	//保存文件插槽名字
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
	
	//-----保存游戏
	UFUNCTION(BlueprintCallable,Category ="SaveGame" )
	void WriteSaveGame();

	UFUNCTION()
	void LoadSaveGame();
};
