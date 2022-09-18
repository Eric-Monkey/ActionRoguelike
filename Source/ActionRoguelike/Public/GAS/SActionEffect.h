// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/SAction.h"
#include "SActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USActionEffect : public USAction
{
	GENERATED_BODY()

public:
	USActionEffect();

	virtual void StartAction_Implementation(AActor* Starter) override;

	virtual void EndAction_Implementation(AActor* Starter) override;

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Effect")
	float Duration; //����Ч������ʱ��

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Effect")
	float Period; //����Ч������ʱ�� ��Ч������ʱ��

	FTimerHandle TimeHandle_Duration;
	FTimerHandle TimerHandle_Period;

	UFUNCTION(BlueprintNativeEvent,Category="Effect")
	void ExecutePeriodEffect(AActor* Instigator);


};
