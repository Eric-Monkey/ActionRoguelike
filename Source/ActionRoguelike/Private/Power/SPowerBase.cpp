// Fill out your copyright notice in the Description page of Project Settings.


#include "Power/SPowerBase.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASPowerBase::ASPowerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;

	respawnTime = 10.0f;
	//SphereComp = RootComponent;
	bIsActive = true;
	bReplicates = true ;
}

void ASPowerBase::OnRep_IsActive()
{
	SetState(bIsActive);
}

// Called when the game starts or when spawned
void ASPowerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASPowerBase::HidePower()
{
	SetState(false);
	GetWorld()->GetTimerManager().SetTimer(timeHandle_show,this,&ASPowerBase::ShowPower,respawnTime);
}

void ASPowerBase::ShowPower()
{
	SetState(true);
	GetWorld()->GetTimerManager().ClearTimer(timeHandle_show);
}

void ASPowerBase::SetState(bool NewState)
{
	bIsActive = NewState;
	SetActorEnableCollision(NewState);
	RootComponent->SetVisibility(NewState, true);
}

// Called every frame
void ASPowerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FText ASPowerBase::GetInteractText_Implementation(APawn* CallPawn)
{
	
	return FText::GetEmpty();
	
}

void ASPowerBase::Interact_Implementation(APawn* CallPawn)
{

}

void ASPowerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerBase,bIsActive);
}

