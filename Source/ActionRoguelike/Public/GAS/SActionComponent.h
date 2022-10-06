// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAction.h"
#include "GameplayTagContainer.h"
#include "SActionComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChange,USActionComponent*, ActionComp, USAction*, Action);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()



protected:
	
	UPROPERTY(Replicated,BlueprintReadOnly,Category="Actionia ")
	TArray<USAction*> Actions;

	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<TSubclassOf<USAction>> DefaultActions;


	UFUNCTION(Server,Reliable)
	void ServerStartActionForName(AActor* Starter, FName ActionName);
	
	UFUNCTION(Server,Reliable)
	void ServerEndActionForName(AActor* Starter, FName ActionName);
public:	
	// Sets default values for this component's properties
	USActionComponent();

	//�����ǩ
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChange OnActionStart;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChange OnActionEnd;

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
