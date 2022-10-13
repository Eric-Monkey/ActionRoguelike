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



public:
	ASChest();

	UPROPERTY(EditAnywhere)
	float LidRotationForX;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer CardTags;

protected:
	//Í¬²½
	UPROPERTY(ReplicatedUsing="OnRep_ChestOpen", BlueprintReadOnly , SaveGame)
	bool bChestOpen;

	UFUNCTION()
	void OnRep_ChestOpen();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* LipMesh;

	virtual void BeginPlay() override;

public:		
	virtual void Interact_Implementation(APawn* CallPawn) override;

	virtual FText GetInteractText_Implementation(APawn* CallPawn) override;
	
	virtual void OnActorLoad_Implementation() override;

	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
