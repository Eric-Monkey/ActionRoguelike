// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_IsWithRange.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USBTService_IsWithRange : public UBTService
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,Category="AI")
	FBlackboardKeySelector IsWithRangeKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector IsBlockKey;
protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
