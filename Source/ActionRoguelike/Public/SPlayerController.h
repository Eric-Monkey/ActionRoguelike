// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "SPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChange, APawn*, InPawn); //用来修复Pawng更新时，UI不更新问题

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChange, APlayerState*, NewPlayerState); //用来修复Pawng更新时，UI不更新问题


/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPawnChange OnPawnChange;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChange OnPlayerStateChange;

	UFUNCTION()
	virtual void SetPawn(APawn* InPawn) override;

	//UFUNCTION(BlueprintNativeEvent)
	//void BlueprintBeginPlayingState();

	//virtual void BeginPlayingState() override;

	void OnRep_PlayerState() override;

protected:
	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY()
	class UUserWidget* PauseWidgetInstance;

	
	UFUNCTION()
	void SetupInputComponent();

	UFUNCTION(BlueprintCallable,Category="UI")
	void TogglePauseMenu();


};
