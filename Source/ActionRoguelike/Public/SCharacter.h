// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Chaos/Array.h"
#include "SCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class USInteractComponent;
class ASBaseProjectile;
class USAttributeComponent;
class USActionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
public:

	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractComponent* InteractComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USActionComponent* ActionComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);

	void MoveRight(float value);
	
	//void PlayAttackAnim();
	//void Attack();
	//void CreateProjectile(TSubclassOf<ASBaseProjectile> SpawnProjectile);


	void StartAction_Sprint();

	void EndAction_Sprint();

	void StartAction_Attack();

	void EndAction_Attack();

	void StartAction_TeleAttack();

	void EndAction_TeleAttack();

	void StartAction_GAttack();

	void EndAction_GAttack();

	void StartAction_Parry();

	void EndAction_Parry();

	UFUNCTION(BlueprintCallable)
	void PrimaryInteract();

	virtual FVector GetPawnViewLocation() const override;

	UFUNCTION()
	void OnHealthChange(AActor* Attacker, USAttributeComponent* AttributeComponent, float health, float ChangeVal);

	UFUNCTION(Exec)
	void Helself(float Val=100);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

};
