// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAction.h"
#include "SActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USActionComponent();

public:
	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<USAction*> Actions;


	UFUNCTION(BlueprintCallable,Category="Action")
	void AddAction(TSubclassOf<USAction> NewAction);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartActionForName(AActor* Starter , FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool EndActionForName(AActor* Starter, FName ActionName);
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
