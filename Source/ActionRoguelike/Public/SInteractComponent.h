// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,Config="Collision")
	float radius;
	void PrimaryInteract();
public:	
	// Sets default values for this component's properties
	USInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
