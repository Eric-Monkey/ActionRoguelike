// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SInterface.h"
#include "SPowerBase.generated.h"


class USphereComponent;
UCLASS()
class ACTIONROGUELIKE_API ASPowerBase : public AActor , public ISInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerBase();

	UFUNCTION()
	void OnRep_IsActive();

protected:

	UPROPERTY()
	FTimerHandle timeHandle_show;
	
	//设置物品可见有碰撞的时间
	UPROPERTY(EditAnywhere,Category="PowerAttribute")
	float respawnTime;

	//可交互状态
	UPROPERTY(ReplicatedUsing="OnRep_IsActive")
	bool bIsActive;
	
	//球形碰撞组件
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;

	virtual void BeginPlay() override;

	void HidePower();

	void ShowPower();

	void SetState(bool NewState);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Interact_Implementation(APawn* CallPawn) override;

};
