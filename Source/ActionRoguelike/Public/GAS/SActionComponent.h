// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAction.h"
#include "GameplayTagContainer.h"
#include "SActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USActionComponent();

protected:
	
	UPROPERTY(Replicated)
		TArray<USAction*> Actions;

	UPROPERTY(EditAnywhere, Category = "Action")
		TArray<TSubclassOf<USAction>> DefaultActions;


	UFUNCTION(Server,Reliable)
	void ServerStartActionForName(AActor* Starter, FName ActionName);
	
	UFUNCTION(Server,Reliable)
	void ServerEndActionForName(AActor* Starter, FName ActionName);


public:	//º§ªÓ±Í«©
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable,Category="ActionEffect")
	bool RemoveAction(USAction* RemoveAction);

	UFUNCTION(BlueprintCallable,Category="Action")
	void AddAction(AActor* Starter, TSubclassOf<USAction> NewAction);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartActionForName(AActor* Starter , FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool EndActionForName(AActor* Starter, FName ActionName);

	UFUNCTION()
	void InitAction();	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

	UFUNCTION(Category = "Action")
	USAction* GetAction( TSubclassOf<USAction> FindAction);

	bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags);
};
