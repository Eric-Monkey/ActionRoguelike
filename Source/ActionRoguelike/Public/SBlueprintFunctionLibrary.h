// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static bool ApplyDamage(AActor* Attack, AActor* TargeActor, float Val);

	static bool ApplyDamageDirection(AActor* Attack, AActor* TargeActor, float Val,const FHitResult& HitResult ,class ASBaseProjectile* BaseProjectile);


};

