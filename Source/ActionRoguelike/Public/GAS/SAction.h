// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "SAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintNativeEvent,Category ="Action")
	void StartAction(AActor* Starter);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void EndAction(AActor* Starter);

	UPROPERTY(EditDefaultsOnly , Category ="Action")
	FName ActionName;

	UWorld* GetWorld() const override;
};
