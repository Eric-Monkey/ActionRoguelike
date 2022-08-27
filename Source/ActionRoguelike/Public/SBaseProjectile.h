// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseProjectile.generated.h"

class UParticleSystem;
class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

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

	FTimerHandle ProjectileHandle;

	UPROPERTY(EditAnywhere, Category = "ParicleEffects")
	UParticleSystem* ParicleEffect;

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

	void SelfDestroy();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
