// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"


class UWorld;
class USActionComponent;

USTRUCT()
struct FACtionRepData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Starter;
};


/**
 *
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated)
	float BeginTime;

	UPROPERTY(Replicated)
	USActionComponent* OwerComp;

	UPROPERTY(ReplicatedUsing="OnRep_IsRunning")
	FACtionRepData RepData;

	UFUNCTION()
	void OnRep_IsRunning();

	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer BlockTags;
	
public:
	USAction();

	UFUNCTION()
	void InitialOwerComp(USActionComponent* NewOwnerComp);

	UPROPERTY(EditAnywhere,Category="Effect")
	bool isAutoStart;

	UFUNCTION(BlueprintNativeEvent,Category = "Action")
	bool CanStart(AActor* Starter = nullptr);

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
	bool IsRunning();

	bool IsSupportedForNetworking() const override {
		return true;
	}
};
