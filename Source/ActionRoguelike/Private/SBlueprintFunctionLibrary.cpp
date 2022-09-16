// Fill out your copyright notice in the Description page of Project Settings.


#include "SBlueprintFunctionLibrary.h"
#include "SAttributeComponent.h"
#include "Components/PrimitiveComponent.h"
#include "SCharacter.h"
#include "SBaseProjectile.h"

bool USBlueprintFunctionLibrary::ApplyDamage(AActor* Attack, AActor* TargeActor, float Val)
{
	USAttributeComponent* AttributeComp=USAttributeComponent::GetAttribute(TargeActor);
	if (AttributeComp) {
		return AttributeComp->ApplyChangeHealth(Attack,-Val);
	}
	return false;
}

bool USBlueprintFunctionLibrary::ApplyDamageDirection(AActor* Attack, AActor* TargeActor, float Val, const FHitResult& HitResult, ASBaseProjectile* BaseProjectile)
{
	USBlueprintFunctionLibrary::ApplyDamage(Attack, TargeActor, Val);
	
	UPrimitiveComponent* PrimitiveComp =HitResult.GetComponent();
	if (PrimitiveComp && PrimitiveComp->IsSimulatingPhysics(HitResult.BoneName)) {
		FVector ImpulseDirect = -HitResult.ImpactNormal;
		if (BaseProjectile) {
			ImpulseDirect = BaseProjectile->GetVelocity() * BaseProjectile->Multiply_Impulse;
		}
		PrimitiveComp->AddImpulseAtLocation(ImpulseDirect, HitResult.ImpactPoint, HitResult.BoneName);
		return true;
	}
	return false;
}
