// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SSaveGame.generated.h"



USTRUCT()
struct  FActorData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString ActorName;

	UPROPERTY()
	FTransform ActorTransform;

	//保存Actor变量的字节数组
	UPROPERTY()
	TArray<uint8> BinActorData;
};
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	int32 Credits;

	UPROPERTY()
	TArray<FActorData> ActorDatas;
};
