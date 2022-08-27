// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseProjectile.h"
#include "SGProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGProjectile : public ASBaseProjectile
{
	GENERATED_BODY()
public:

	ASGProjectile();

	virtual void Explode_Implementation() override;

	virtual void BeginPlay() override;
	
};
