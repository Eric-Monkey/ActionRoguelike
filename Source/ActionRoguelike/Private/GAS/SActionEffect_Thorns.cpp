// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/SActionEffect_Thorns.h"
#include "SAttributeComponent.h"
#include "GAS/SAction.h"
#include "GAS/SActionComponent.h"
#include "SBlueprintFunctionLibrary.h"

USActionEffect_Thorns::USActionEffect_Thorns()
{
	Duration = 0.0f;
	Period = 0.0f;

	ReflectPercent = 0.2f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Starter)
{
	Super::StartAction_Implementation(Starter);

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttribute(GetOwnerActionComp()->GetOwner());
	if (AttributeComp) {
		AttributeComp->ApplyHealthChange.AddDynamic(this,&USActionEffect_Thorns::OnHealthChange);
	}
}

void USActionEffect_Thorns::EndAction_Implementation(AActor* Starter)
{
	Super::EndAction_Implementation(Starter);

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttribute(Starter);
	if (AttributeComp) {
		AttributeComp->ApplyHealthChange.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChange);
	}
}

void USActionEffect_Thorns::OnHealthChange(AActor* Attacker, USAttributeComponent* AttributeComponent, float health, float ChangeVal)
{
	//获取开启能力者
	AActor* Ownering = GetOwnerActionComp()->GetOwner();
	//开启能力 和 攻击者一样 不反弹

	if (ChangeVal<0 && Ownering != Attacker) {
		int32 ReflectAmount = FMath::RoundToInt(ReflectPercent * ChangeVal); //计算反弹伤害量

		if (ReflectAmount==0) {
			return;	//计算出反弹量为0,返回
		}
		UE_LOG(LogTemp,Log,TEXT("ReflectAmount:%i"), ReflectAmount)

		//获取攻击者，造成伤害
		if (Ownering->HasAuthority()) {
		USBlueprintFunctionLibrary::ApplyDamage(Ownering, Attacker,FMath::Abs(ReflectAmount));
		}

	}
}


