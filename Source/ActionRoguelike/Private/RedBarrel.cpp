// Fill out your copyright notice in the Description page of Project Settings.


#include "RedBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "DrawDebugHelpers.h"


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
	StaticMeshComp->OnComponentHit.AddDynamic(this,&ARedBarrel::OnActorHit);
}

void ARedBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	RadialForceComp->FireImpulse();
	
	UE_LOG(LogTemp,Log,TEXT("_________________________________Boom"));

	UE_LOG(LogTemp, Warning, TEXT("OtherActor %s	Time:%f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	FString Text = FString::Printf(TEXT("Hit at location:%s"),*Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(),Hit.ImpactPoint,Text,nullptr,FColor::Green,2,true,1);
}

// Called every frame
void ARedBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

