// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHitFlashItem.generated.h"

class UStaticMeshComponent;
class USAttributeComponent;
UCLASS()
class ACTIONROGUELIKE_API ASHitFlashItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASHitFlashItem();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	USAttributeComponent* AttributeComp;

	UFUNCTION()
	void OnHealthChange(AActor* Attacker, USAttributeComponent* AttributeComponent, float health, float ChangeVal);
};
