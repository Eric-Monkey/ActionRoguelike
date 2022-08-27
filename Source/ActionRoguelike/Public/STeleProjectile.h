// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseProjectile.h"
#include "STeleProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASTeleProjectile : public ASBaseProjectile
{
	GENERATED_BODY()


public:
	ASTeleProjectile();
//	
	protected:
	virtual void Explode_Implementation() override;

	virtual void BeginPlay() override;

	void SelfDestroy();


	UFUNCTION()
	void TelePawn();
};
