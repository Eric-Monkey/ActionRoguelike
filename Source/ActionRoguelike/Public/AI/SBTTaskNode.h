// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBaseProjectile.h"
#include "SBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	USBTTaskNode();
protected:
	UPROPERTY(EditAnywhere,Category="AI")
	TSubclassOf<ASBaseProjectile> ProjectileEffect;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
