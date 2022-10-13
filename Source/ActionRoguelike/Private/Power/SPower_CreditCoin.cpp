// Fill out your copyright notice in the Description page of Project Settings.


#include "Power/SPower_CreditCoin.h"
#include "SPlayerState.h"
#include "Components/StaticMeshComponent.h"

ASPower_CreditCoin::ASPower_CreditCoin()
{
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->SetupAttachment(RootComponent);

	CoinCreits = 50;
}

FText ASPower_CreditCoin::GetInteractText_Implementation(APawn* CallPawn)
{
	return FText::Format(NSLOCTEXT("InteractableActor", "CreditCoinPotion_InteractMsg", "Grant {0} Credits."), CoinCreits);
}

void ASPower_CreditCoin::Interact_Implementation(APawn* CallPawn)
{
	if (!CallPawn) {
		return;
	}
	ASPlayerState* PS = CallPawn->GetPlayerState<ASPlayerState>();
	if (PS) {
		PS->AddCredits(CoinCreits);
		HidePower();
	}
}
