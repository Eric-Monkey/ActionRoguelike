// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/SAction.h"
#include "GAS/SActionComponent.h"


USAction::USAction()
{
	isAutoStart = false;
}

USActionComponent* USAction::GetOwnerActionComp() const
{
	USActionComponent* ActionComp = Cast<USActionComponent>(GetOuter());
	if (ensure(ActionComp)) {
		return ActionComp;
	}
	return nullptr;
}

bool USAction::CanStart_Implementation()
{
	if (IsRuning()) {
		return false;
	}

	if (GetOwnerActionComp()->ActiveGameplayTags.HasAny(BlockTags)) {
		return false;
	}
	return true;
}

void USAction::StartAction_Implementation(AActor* Starter)
{	
	/*UE_LOG(LogTemp, Warning, TEXT("StartAction:%s"), *ActionName.ToString());*/
	USActionComponent* ActionComp = GetOwnerActionComp();
	ActionComp->ActiveGameplayTags.AppendTags(GrantsTags);
	bIsRuning = true;
}

void USAction::EndAction_Implementation(AActor* Starter)
{
	USActionComponent* ActionComp = GetOwnerActionComp();
	ActionComp->ActiveGameplayTags.RemoveTags(GrantsTags);
	bIsRuning = false;
}

UWorld* USAction::GetWorld() const
{
	USActionComponent* ActionComp = Cast<USActionComponent>(GetOuter());
	if (ActionComp) {
		return ActionComp->GetWorld();
	}
	return nullptr;
}

bool USAction::IsRuning()
{
	return bIsRuning;
}