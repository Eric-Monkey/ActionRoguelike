// Fill out your copyright notice in the Description page of Project Settings.


#include "SAnimInstance.h"
#include "GameplayTagContainer.h"
#include "GAS/SActionComponent.h"

void USAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwnningActor = GetOwningActor();
	if (OwnningActor)
	{
		OwnningActionComp = Cast<USActionComponent>(OwnningActor->GetComponentByClass(USActionComponent::StaticClass())) ;
	}
}

void USAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("State.Stunned");

	if (OwnningActionComp) {
		bIsStunned = OwnningActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
