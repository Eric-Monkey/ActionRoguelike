// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

ASTeleProjectile::ASTeleProjectile() {
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	LifeTime = 0.5f;
	ProjectileMoveComp->InitialSpeed = 1500;
}

void ASTeleProjectile::Explode_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("CallTele exploer"));
	this->TelePawn();
}

void ASTeleProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ASTeleProjectile::SelfDestroy()
{
	this->TelePawn();
	GetWorld()->GetTimerManager().ClearTimer(ProjectileHandle);
}

void ASTeleProjectile::TelePawn()
{
	APawn* mypawn = GetInstigator();
	UGameplayStatics::SpawnEmitterAtLocation(this,BegEffect,GetActorLocation(),GetActorRotation());
	mypawn->TeleportTo(GetActorLocation(), mypawn->GetActorRotation(), false, false);
	UGameplayStatics::SpawnEmitterAtLocation(this, EndEffect, GetActorLocation(), GetActorRotation());
	Destroy();
}

//void ASTeleProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	if (Hit.bBlockingHit)
//}

