// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Power/SPowerBase.h"
#include "SPower_CreditCoin.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPower_CreditCoin : public ASPowerBase
{
	GENERATED_BODY()

protected:
	//Ӳ�Ҽӵķ���
	UPROPERTY()
	int32 CoinCreits;

	//��̬������
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UStaticMeshComponent* StaticMeshComp;

public:

	ASPower_CreditCoin();

	virtual void Interact_Implementation(APawn* CallPawn) override;

	UFUNCTION()
	FText GetInteractText_Implementation(APawn* CallPawn) override;

};
