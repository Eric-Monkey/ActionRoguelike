// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SInterface.h"
#include "GameplayTagContainer.h"
#include "SChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASChest : public AActor, public ISInterface
{
	GENERATED_BODY()

	void Interact_Implementation(APawn* CallPawn);

public:
	UPROPERTY(EditAnywhere)
	float LidRotationForX;


	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer CardTags;
	
public:	
	// Sets default values for this actor's properties
	ASChest();

protected:
	

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LipMesh;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
