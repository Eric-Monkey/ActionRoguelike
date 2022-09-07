// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_IsLowHealth.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_IsLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIPc=Cast<AAIController>(OwnerComp.GetAIOwner());
	if (AIPc) {
		APawn* AIPawn= AIPc->GetPawn();
		USAttributeComponent* AttributeComp=USAttributeComponent::GetAttribute(AIPawn);
		
		bool IsLowHealth =  (AttributeComp->GetHealth()) <= HealthToHide;

		UBlackboardComponent* BlackboardComp =OwnerComp.GetBlackboardComponent();
		BlackboardComp->SetValueAsBool(IsLowHealthSelected.SelectedKeyName,IsLowHealth);
	}
	
}
