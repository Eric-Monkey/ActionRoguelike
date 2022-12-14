// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Power/SPowerBase.h"
#include "SPower_Health.generated.h"

/**
 * 
 */

class UStaticMeshComponent;
UCLASS()
class ACTIONROGUELIKE_API ASPower_Health : public ASPowerBase
{
	GENERATED_BODY()
public:
	ASPower_Health();
protected:
	//使用花费
	UPROPERTY(EditAnywhere,Category="Credits")
	int32	CreditsCast;
	//治疗量
	UPROPERTY(EditAnywhere,Category="Attribute")
	float treatMax;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComp;
	
	UFUNCTION()
	void Interact_Implementation(APawn* CallPawn);

	UFUNCTION()
	FText GetInteractText_Implementation(APawn* CallPawn) override;
};
