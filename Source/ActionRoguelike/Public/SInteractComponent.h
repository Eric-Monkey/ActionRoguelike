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
	//���뾶
	UPROPERTY(EditAnywhere, Category = "InteractTrace")
	float Traceradius;

	//������
	UPROPERTY(EditAnywhere, Category = "InteractTrace")
	float TraceDistance;
	UPROPERTY(EditAnywhere, Category = "InteractTrace")
	TEnumAsByte<ECollisionChannel> TraceTypes;
	
	//����
	UPROPERTY()
	AActor* FocusedActor;

	//��ʾUI��
	UPROPERTY(EditDefaultsOnly,Category="InteractTrace")
	TSubclassOf<UMyUserWidget> InteractUIClass;
	//�����ഴ���ĵ���
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
