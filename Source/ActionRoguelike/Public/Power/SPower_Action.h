// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Power/SPowerBase.h"
#include "SPower_Action.generated.h"

/**
 * 
 */
class USAction;
class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASPower_Action : public ASPowerBase
{
	GENERATED_BODY()
	
public:
	ASPower_Action();

	virtual void Interact_Implementation(APawn* CallPawn) override;

	virtual FText GetInteractText_Implementation(APawn* CallPawn) override;

protected:
	UPROPERTY(EditDefaultsOnly,Category="PowerAction")
	FString GrantActionName;

	UPROPERTY(VisibleAnywhere,Category="PowerAction")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, Category="PowerAction")
	TSubclassOf<USAction> GrantActionClass;
};
