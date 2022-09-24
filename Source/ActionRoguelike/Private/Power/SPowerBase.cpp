// Fill out your copyright notice in the Description page of Project Settings.


#include "Power/SPowerBase.h"
#include "Components/SphereComponent.h"

// Sets default values
ASPowerBase::ASPowerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;

	respawnTime = 10.0f;
	//SphereComp = RootComponent;
	SetReplicates(true);
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

void ASPowerBase::SetState(bool state)
{
	SetActorEnableCollision(state);

	RootComponent->SetVisibility(state, true);
}

// Called every frame
void ASPowerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPowerBase::Interact_Implementation(APawn* CallPawn)
{

}

