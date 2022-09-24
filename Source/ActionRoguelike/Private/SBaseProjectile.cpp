// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
ASBaseProjectile::ASBaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
	ProjectileMoveComp->InitialSpeed = 3000.0f;
	ProjectileMoveComp->ProjectileGravityScale = 0;

	//ÉùÒô×é¼þ
	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(SphereComp);

	Multiply_Impulse = 1;
}

void ASBaseProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}



void ASBaseProjectile::Explode_Implementation()
{
	//±¬Õ¨
	if (ensure(ParicleEffect)) {
		UGameplayStatics::SpawnEmitterAtLocation(this, ParicleEffect, GetActorLocation());
	}
	//ÉùÒô
	if (ensure(ImpactCue)) {
		UGameplayStatics::PlaySoundAtLocation(this, ImpactCue, GetActorLocation());
		UGameplayStatics::PlayWorldCameraShake(this, CameraShake, GetActorLocation(), 0, 300);
	}

	Destroy();
}


void ASBaseProjectile::SelfDestroy()
{
	Destroy();
	GetWorld()->GetTimerManager().ClearTimer(ProjectileHandle);
}

// Called when the game starts or when spawned
void ASBaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	GetWorld()->GetTimerManager().SetTimer(ProjectileHandle,this,&ASBaseProjectile::SelfDestroy,LifeTime);
}

// Called every frame
void ASBaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


