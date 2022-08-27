// Fill out your copyright notice in the Description page of Project Settings.


#include "SGProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

ASGProjectile::ASGProjectile() {
	LifeTime = 5;
	ProjectileMoveComp->InitialSpeed = 800;
}
void ASGProjectile::Explode_Implementation()
{
	
}

void ASGProjectile::BeginPlay()
{
	Super::BeginPlay();
}
