// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SBaseProjectile.h"
#include "SBlueprintFunctionLibrary.h"
#include "GAS/SActionComponent.h"
// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	ProjectileMoveComp->InitialSpeed = 2000.0f;
	Damage = 20;
}

void ASMagicProjectile::OnCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (GetInstigator()) {
		if (OtherActor->GetClass() == GetInstigator()->GetClass())//同类无伤
		{
			return;
		}
	}
	if (OtherActor && OtherActor != GetInstigator()) {
		//判定是否被格挡
		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass())) ;
		
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag)) {
			
			ProjectileMoveComp->Velocity = -(ProjectileMoveComp->Velocity);
			this->SetInstigator(Cast<APawn>(OtherActor));

			return;
		}

		//爆炸
		if (ensure(ParicleEffect)) {
			UGameplayStatics::SpawnEmitterAtLocation(this, ParicleEffect, GetActorLocation());
		}
		//声音
		if (ensure(ImpactCue)) {
			UGameplayStatics::PlaySoundAtLocation(this, ImpactCue, GetActorLocation());
			UGameplayStatics::PlayWorldCameraShake(this, CameraShake, GetActorLocation(), 0, 300);
		}
		//伤害
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp) {
			/*AttributeComp->ApplyChangeHealth(GetInstigator(), -Damage);
			Destroy();*/
			USBlueprintFunctionLibrary::ApplyDamageDirection(this->GetInstigator(), OtherActor, Damage ,SweepResult,this);
			
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

