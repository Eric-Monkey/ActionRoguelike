// Fill out your copyright notice in the Description page of Project Settings.


#include "Power/SPower_Action.h"
#include "SCharacter.h"
#include "GAS/SActionComponent.h"
#include "Components/StaticMeshComponent.h"

//Run on Server
void ASPower_Action::Interact_Implementation(APawn* CallPawn)
{

	USActionComponent* ActionComponent = Cast<USActionComponent>(CallPawn->GetComponentByClass(USActionComponent::StaticClass()));

	//情况合法
	if ( ActionComponent && GrantActionClass) {

		//Server have Action already
		if (ActionComponent->GetAction(GrantActionClass)) {

			FString DebugMsg = FString::Printf( TEXT("Action:%s is already known. "),*GetNameSafe(GrantActionClass)  );
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			return; 
		}
		//Server没有Action则添加
		ActionComponent->AddAction(CallPawn , GrantActionClass);
	}	
}


ASPower_Action::ASPower_Action()
{
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->SetupAttachment(RootComponent);
}
