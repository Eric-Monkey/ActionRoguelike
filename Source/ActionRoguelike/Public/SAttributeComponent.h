// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FApplyAttributeChange, AActor*,Attacker,USAttributeComponent*,AttributeComponent, float,health, float,ChangeVal);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

	UFUNCTION(BlueprintCallable,Category="Attribute")
	static USAttributeComponent* GetAttribute(AActor* Actor);
protected:


	UPROPERTY(Replicated,EditDefaultsOnly,BlueprintReadOnly,Category="Attributes")
	float Health;
	UPROPERTY(Replicated,EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth;

	//Å­Æø
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Rage;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	float MaxRage;

	UFUNCTION(NetMulticast,Reliable)
	void NetMulticastApplyHealthChange(AActor* Attacker,float NewHealth , float ChangeVal , bool IsFromServer=true);

public:	
	UPROPERTY(BlueprintAssignable)
	FApplyAttributeChange ApplyHealthChange;

	UPROPERTY(BlueprintAssignable)
	FApplyAttributeChange ApplyRageChange;

	UFUNCTION(BlueprintCallable)
	bool ApplyChangeHealth(AActor* Attack,float Val);

	UFUNCTION(BlueprintCallable)
	bool ApplyChangeRage(AActor* Attack, float Val);

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* Attack);
	UFUNCTION()
	bool IsAlive();

	UFUNCTION()
	bool IsFullHealth();

	UFUNCTION()
	float GetMaxHealth();

	UFUNCTION()
	float GetHealth();


	UFUNCTION()
	float GetRage();

	UFUNCTION()
	float GetMaxRage();

};

