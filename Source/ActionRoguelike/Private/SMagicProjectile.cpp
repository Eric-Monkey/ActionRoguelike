// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SBaseProjectile.h"
// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	ProjectileMoveComp->InitialSpeed = 2000.0f;
	Damage = 20;
}

void ASMagicProjectile::OnCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (GetInstigator()) {
		if (OtherActor->GetClass() == GetInstigator()->GetClass())//Õ¨¿‡Œﬁ…À
		{
			return;
		}
	}
	if (OtherActor && OtherActor != GetInstigator()) {
		if (ensure(ParicleEffect)) {
			UGameplayStatics::SpawnEmitterAtLocation(this, ParicleEffect, GetActorLocation());
		}
		if (ensure(ImpactCue)) {
			UGameplayStatics::PlaySoundAtLocation(this, ImpactCue, GetActorLocation());
			UGameplayStatics::PlayWorldCameraShake(this, CameraShake, GetActorLocation(), 0, 300);
		}
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp) {
			AttributeComp->ApplyChangeHealth(GetInstigator(), -Damage);
			Destroy();
		}
	}
	
	
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&ASMagicProjectile::OnCompBeginOverlap);
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

