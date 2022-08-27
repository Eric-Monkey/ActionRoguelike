// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class USInteractComponent;
class ASBaseProjectile;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Projectile;
	/*UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BaseProjectile;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> TeleProjectile;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> GProjectile;*/
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackMontage;

	bool IsAttack;

	FTimerHandle AttackTimerHandle;

	void SetIsAttack();
	
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractComponent* InteractComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);

	void MoveRight(float value);
	
	void PlayAttackAnim();

	void CreateProjectile(TSubclassOf<AActor> SpawnProjectile);

	void UseBaseProjectile();

	void UseTeleProjectile();

	void UseGProjectile();

	UFUNCTION(BlueprintCallable)
	void Attack();

	void PrimaryInteract();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};
