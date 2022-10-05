// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"
#include "GameFramework/PlayerController.h"

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ASPlayerController::TogglePauseMenu);
}

void ASPlayerController::TogglePauseMenu()
{
	if (PauseWidgetInstance && PauseWidgetInstance->IsInViewport()) {
		PauseWidgetInstance->RemoveFromParent();
		bShowMouseCursor = false;
		PauseWidgetInstance = nullptr;
		SetInputMode(FInputModeGameOnly());
		return;
	}
	
	if (PauseWidgetInstance==nullptr && ensure(PauseWidgetClass)) {

		PauseWidgetInstance = CreateWidget<UUserWidget>(this, PauseWidgetClass);
		if (PauseWidgetInstance) {
			PauseWidgetInstance->AddToViewport(100);
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}


void ASPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChange.Broadcast(InPawn);
}

void ASPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	OnPlayerStateChange.Broadcast(GetPlayerState<APlayerState>());
}


//void ASPlayerController::BlueprintBeginPlayingState_Implementation()
//{
//
//}
//void ASPlayerController::BeginPlayingState()
//{
//	Super::BeginPlayingState();
//	BlueprintBeginPlayingState();
//}
