// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/SAction.h"
#include "GAS/SActionComponent.h"
#include "Net/UnrealNetwork.h"
#include "MyGameModeBase.h"



USAction::USAction()
{
	isAutoStart = false;

}

void USAction::InitialOwerComp(USActionComponent* NewOwnerComp)
{
	OwerComp = NewOwnerComp;
}

USActionComponent* USAction::GetOwnerActionComp() const
{
	return OwerComp;
}



bool USAction::CanStart_Implementation(AActor* Starter)// Default Starter = nullptr
{
	if (IsRunning()) {
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
	
	ActionComp->ActiveGameplayTags.AppendTags(GrantsTags);	//赋予标签

	RepData.bIsRunning = true;	
	RepData.Starter = Starter;	//设置行动发起者

	if (GetOwnerActionComp()->GetOwnerRole()==ROLE_Authority) {

		BeginTime = GetWorld()->TimeSeconds; //设置开始时间
	}
	ActionComp->OnActionStart.Broadcast(ActionComp, this);
}

void USAction::EndAction_Implementation(AActor* Starter)
{
	USActionComponent* ActionComp = GetOwnerActionComp();
	ActionComp->ActiveGameplayTags.RemoveTags(GrantsTags);
	ActionComp->OnActionEnd.Broadcast(ActionComp, this);

	RepData.bIsRunning = false;
	RepData.Starter = Starter;
}

UWorld* USAction::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor) {
		return Actor->GetWorld();
	}
	return nullptr;
}

void USAction::OnRep_IsRunning()
{
	if (RepData.bIsRunning) {

		StartAction(RepData.Starter);
	}
	else
	{
		EndAction(RepData.Starter);
	}

}

bool USAction::IsRunning()
{
	return RepData.bIsRunning;
}

void USAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);

	DOREPLIFETIME(USAction, OwerComp);
	DOREPLIFETIME(USAction, BeginTime);
}