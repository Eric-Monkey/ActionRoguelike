// Fill out your copyright notice in the Description page of Project Settings.


#include "RedBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ARedBarrel::ARedBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMeshComp;
	StaticMeshComp->SetSimulatePhysics(true);

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(StaticMeshComp);
	RadialForceComp->Radius = 800;
	RadialForceComp->ImpulseStrength = 1000;
	RadialForceComp->bIgnoreOwningActor = true;
	RadialForceComp->bImpulseVelChange = true;
	
}

// Called when the game starts or when spawned
void ARedBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARedBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

