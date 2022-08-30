// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseProjectile.generated.h"

class UParticleSystem;
class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class USoundCue;
class UAudioComponent;
class USoundBase;

UCLASS()
class ACTIONROGUELIKE_API ASBaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASBaseProjectile();

protected:
	UPROPERTY(EditAnywhere,Category = "ParicleEffects")
	float LifeTime;

	UPROPERTY()
	FTimerHandle ProjectileHandle;

	UPROPERTY(EditAnywhere, Category = "ParticleEffects")
	UParticleSystem* ParicleEffect;

	UPROPERTY(EditAnywhere, Category = "ParticleEffects")
	USoundBase* ImpactCue; 

	//×é¼þ
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="components")
	UAudioComponent* AudioComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "components")
	UProjectileMovementComponent* ProjectileMoveComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "components")
	UParticleSystemComponent* ParticleSystemComp;

	UFUNCTION()
	virtual void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	UFUNCTION()
	virtual void SelfDestroy();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
