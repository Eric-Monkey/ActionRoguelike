// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

/**
 * 
 */
class UWorld;
class USActionComponent;

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	bool bIsRuning;

	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer BlockTags;
	
public:
	UFUNCTION(BlueprintNativeEvent,Category = "Action")
	bool CanStart(AActor* Starter);

	UFUNCTION(BlueprintNativeEvent,Category ="Action")
	void StartAction(AActor* Starter);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable ,Category = "Action")
	void EndAction(AActor* Starter);

	UPROPERTY(EditDefaultsOnly , Category ="Action")
	FName ActionName;

	UWorld* GetWorld() const override;

	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOwnerActionComp() const;

	UFUNCTION(BlueprintCallable,Category="Action")
	bool IsRuning();
};
