// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_IsWithRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "AIController.h"

void USBTService_IsWithRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory,DeltaSeconds);

	//比较2个actor的距离,一个是黑板，一个是自己
	UBlackboardComponent* BlackboardComp=OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp)) {
		AActor* ToActor = Cast<AActor>(BlackboardComp->GetValueAsObject("ToActor"));
		if (ToActor) {
			AAIController* AIController = OwnerComp.GetAIOwner();
			if (ensure(AIController)) {
				APawn* AIPawn = AIController->GetPawn();
				if (ensure(AIPawn))
				{
					float distance = FVector::Distance(AIPawn->GetActorLocation(), ToActor->GetActorLocation());
					bool IsWithRange = distance < 1000;
					bool IsBlack = false;
					if (IsWithRange)
						IsBlack=AIController->LineOfSightTo(ToActor);

						BlackboardComp->SetValueAsBool(IsWithRangeKey.SelectedKeyName, IsBlack&&IsWithRange);
						//BlackboardComp->SetValueAsBool(IsBlockKey.SelectedKeyName,IsBlack);

				}
			}
		}
	}	
}
