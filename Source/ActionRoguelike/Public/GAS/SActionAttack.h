// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GAS/SAction.h"
#include "SActionAttack.generated.h"

/**
 * 
 */
class UAnimMontage;
class UParticleSystem;
class ASBaseProjectile;

UCLASS()
class ACTIONROGUELIKE_API USActionAttack : public USAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category="Attack")
	UParticleSystem* AttackParticle;

	UPROPERTY(EditAnywhere,Category = "Attack")
	TSubclassOf<ASBaseProjectile> CurrentProjectile;

	UPROPERTY(EditDefaultsOnly,Category="Attack")
	FName AttackSocket;

	UPROPERTY(EditAnywhere,Category="Attack")
	float AttackDelayTime;

public: 
	USActionAttack();

	UFUNCTION()
	virtual void StartAction_Implementation(AActor* Starter) override;

	
	UFUNCTION()
	void AttackDelay(ACharacter* Starter);

};


