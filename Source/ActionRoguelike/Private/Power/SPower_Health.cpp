// Fill out your copyright notice in the Description page of Project Settings.


#include "Power/SPower_Health.h"
#include "Components/StaticMeshComponent.h"
#include "SAttributeComponent.h"

ASPower_Health::ASPower_Health() {
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComp->SetupAttachment(RootComponent);
	treatMax = 100;
}

void ASPower_Health::Interact_Implementation(APawn* CallPawn)
{
	if (!ensure(CallPawn)) return;
	USAttributeComponent* AttributeComp=Cast<USAttributeComponent>(CallPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth()) {
		
		AttributeComp->ApplyChangeHealth(this,treatMax);
		HidePower();
	}

}

