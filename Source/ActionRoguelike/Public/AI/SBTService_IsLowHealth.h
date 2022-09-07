// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_IsLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USBTService_IsLowHealth : public UBTService
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector IsLowHealthSelected;

	UPROPERTY(EditAnywhere,Category="AI")
	float HealthToHide=20;
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
