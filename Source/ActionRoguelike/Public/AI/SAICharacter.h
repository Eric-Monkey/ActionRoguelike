// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionTypes.h"
#include "SAttributeComponent.h"
#include "SAICharacter.generated.h"


class UAIPerceptionComponent;
class USAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	UPROPERTY(VisibleAnywhere,Category="AI")
	UAIPerceptionComponent*  AIPerceptionComponent;

	UPROPERTY(EditAnywhere,Category="Attribute")
	USAttributeComponent* AttributeComp;
public:
	UFUNCTION()
	bool IsAlive();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnTarPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);
};
