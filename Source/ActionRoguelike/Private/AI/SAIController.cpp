// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(BehaviorTree))
	{
		RunBehaviorTree(BehaviorTree);
	}
	//APawn* MyPawn=UGameplayStatics::GetPlayerPawn(this,0);
	//if (MyPawn) {
	//	//GetBlackboardComponent()->SetValueAsVector("Location",MyPawn->GetActorLocation());
	//	GetBlackboardComponent()->SetValueAsObject("ToActor", MyPawn);
	//}
}
