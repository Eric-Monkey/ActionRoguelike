// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/SActionEffect.h"
#include "SActionEffect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USActionEffect_Thorns : public USActionEffect
{
	GENERATED_BODY()
	
public:

	USActionEffect_Thorns();

	virtual void StartAction_Implementation(AActor* Starter) override;

	virtual void EndAction_Implementation(AActor* Starter) override;


protected:
	UPROPERTY(EditDefaultsOnly,Category="Thorns")
	float ReflectPercent;
		
	UFUNCTION()
	void OnHealthChange(AActor* Attacker, USAttributeComponent* AttributeComponent, float health, float ChangeVal);
};
