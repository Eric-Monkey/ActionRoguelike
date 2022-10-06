// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/SActionEffect.h"
#include "GAS/SActionComponent.h"
#include "GameFramework/GameStateBase.h"

USActionEffect::USActionEffect()
{
	Duration = 3.0f;
	Period = 1.0f;
}

float USActionEffect::GetRemainningTime()
{
	AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>();
	if (GS) {

		float endTime = BeginTime + Duration;
		return endTime - GS->GetServerWorldTimeSeconds();
	}
	return Duration;
}

void USActionEffect::StartAction_Implementation(AActor* Starter)
{
	Super::StartAction_Implementation(Starter);

	//����Ч������,����EndAction;
	if (Duration>0.0f) {
		
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "EndAction", Starter);
		GetWorld()->GetTimerManager().SetTimer(TimeHandle_Duration, Delegate, Duration, false);
	}
	
	//Action û������ִ��Ч��
	if (Period>0.0f) {
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodEffect", Starter);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Period, Delegate, Period, true);
	}

}

void USActionEffect::EndAction_Implementation(AActor* Starter)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_Period) < KINDA_SMALL_NUMBER) {
		ExecutePeriodEffect(Starter);
	}

	Super::EndAction_Implementation(Starter);
	
	//Duration������Period֮ǰ������������periodҪ����֮ǰ����һ��.
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Period);
	GetWorld()->GetTimerManager().ClearTimer(TimeHandle_Duration); 
	
	//��Action����������Ƴ�
	USActionComponent* ActionComp=Cast<USActionComponent>(GetOwnerActionComp());
	ActionComp->RemoveAction(this);
}


void USActionEffect::ExecutePeriodEffect_Implementation(AActor* Instigator)
{

}
