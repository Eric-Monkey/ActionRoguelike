// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/SAction.h"
#include "GAS/SActionComponent.h"

void USAction::StartAction_Implementation(AActor* Starter)
{

}

void USAction::EndAction_Implementation(AActor* Starter)
{

}

UWorld* USAction::GetWorld() const
{
	USActionComponent* ActionComp = Cast<USActionComponent>(GetOuter());
	if (ActionComp) {
		return ActionComp->GetWorld();
	}
	return nullptr;
}
