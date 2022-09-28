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
	//��ȡ����������
	AActor* Ownering = GetOwnerActionComp()->GetOwner();
	//�������� �� ������һ�� ������

	if (ChangeVal<0 && Ownering != Attacker) {
		int32 ReflectAmount = FMath::RoundToInt(ReflectPercent * ChangeVal); //���㷴���˺���

		if (ReflectAmount==0) {
			return;	//�����������Ϊ0,����
		}
		UE_LOG(LogTemp,Log,TEXT("ReflectAmount:%i"), ReflectAmount)

		//��ȡ�����ߣ�����˺�
		if (Ownering->HasAuthority()) {
		USBlueprintFunctionLibrary::ApplyDamage(Ownering, Attacker,FMath::Abs(ReflectAmount));
		}

	}
}


