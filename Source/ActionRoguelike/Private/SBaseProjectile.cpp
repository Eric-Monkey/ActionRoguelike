// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASBaseProjectile::ASBaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LifeTime = 4;
	SphereComp=CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->OnComponentHit.AddDynamic(this, &ASBaseProjectile::OnComponentHit);
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->SetSphereRadius(16);
	RootComponent = SphereComp;

	ParticleSystemComp=CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComp");
	ParticleSystemComp->SetupAttachment(SphereComp);

	ProjectileMoveComp=CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMoveComp");
	ProjectileMoveComp->bRotationFollowsVelocity = true;
	ProjectileMoveComp->bInitialVelocityInLocalSpace = true;
	ProjectileMoveComp->InitialSpeed = 2000.0f;
	ProjectileMoveComp->ProjectileGravityScale = 0;

}

void ASBaseProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}



void ASBaseProjectile::Explode_Implementation()
{

	if (ensure(!IsPendingKill())) {
		UGameplayStatics::SpawnEmitterAtLocation(this, ParicleEffect, GetActorLocation(), GetActorRotation());
		Destroy();
	}

}

void ASBaseProjectile::SelfDestroy()
{
	UE_LOG(LogTemp, Warning, TEXT("_______31"));
	Destroy();
	GetWorld()->GetTimerManager().ClearTimer(ProjectileHandle);
}

// Called when the game starts or when spawned
void ASBaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp,Warning,TEXT("_______1"));
	
	GetWorld()->GetTimerManager().SetTimer(ProjectileHandle,this,&ASBaseProjectile::SelfDestroy,LifeTime);
	UE_LOG(LogTemp, Warning, TEXT("_______12"));
}

// Called every frame
void ASBaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

