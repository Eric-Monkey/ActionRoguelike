// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/MyUserWidget.h"
#include "Engine/EngineTypes.h"
#include "SInteractComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	USInteractComponent();

protected:
	//检测半径
	UPROPERTY(EditAnywhere, Category = "InteractTrace")
	float Traceradius;

	//检测距离
	UPROPERTY(EditAnywhere, Category = "InteractTrace")
	float TraceDistance;
	UPROPERTY(EditAnywhere, Category = "InteractTrace")
	TEnumAsByte<ECollisionChannel> TraceTypes;
	
	//焦点
	UPROPERTY()
	AActor* FocusedActor;

	//显示UI类
	UPROPERTY(EditDefaultsOnly,Category="InteractTrace")
	TSubclassOf<UMyUserWidget> InteractUIClass;
	//根据类创建的单例
	UPROPERTY()
	class UMyUserWidget* InteractUIInstence;

public:	

	UFUNCTION()
	void PrimaryInteract();

	UFUNCTION()
	void BesterInteract();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Called when the game starts
	virtual void BeginPlay() override;
};
