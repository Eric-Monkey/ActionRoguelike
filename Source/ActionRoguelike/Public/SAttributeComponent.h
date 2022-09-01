// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FApplyHealthChange, AActor*,Attacker,USAttributeComponent*,AttributeComponent, float,health, float,ChangeVal);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Attributes")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth;

public:	
	UPROPERTY(BlueprintAssignable)
	FApplyHealthChange ApplyHealthChange;

	UFUNCTION()
	bool IsAlive();

	UFUNCTION()
	bool IsFullHealth();

	UFUNCTION()
	float GetMaxHealth();

	UFUNCTION()
	float GetHealth();

	UFUNCTION(BlueprintCallable)
	bool ApplyChangeHealth(float Val);
};
