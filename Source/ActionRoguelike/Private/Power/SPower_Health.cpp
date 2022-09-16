// Fill out your copyright notice in the Description page of Project Settings.


#include "Power/SPower_Health.h"
#include "Components/StaticMeshComponent.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

ASPower_Health::ASPower_Health() {
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComp->SetupAttachment(RootComponent);
	treatMax = 100;
	CreditsCast = 50;
}

void ASPower_Health::Interact_Implementation(APawn* CallPawn)
{
	if (!ensure(CallPawn)) return;

	USAttributeComponent* AttributeComp=Cast<USAttributeComponent>(CallPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	//判断是否有属性组件，血量是否满
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth()) {
		//判断分数是否足够
		ASPlayerState* PS = (CallPawn->GetPlayerState<ASPlayerState>());

		if (PS && PS->RemoveCredits(CreditsCast) && AttributeComp->ApplyChangeHealth(this, treatMax)) {			
			HidePower();
		}
	}
}

