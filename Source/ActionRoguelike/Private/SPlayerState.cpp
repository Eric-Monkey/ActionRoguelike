// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "SaveGame/SSaveGame.h"

void ASPlayerState::AddCredits(int32 Delta)
{
	if (Delta <= 0) {
		return;
	}
	Credits += Delta;

	OnCreditsChanged.Broadcast(this, Credits , Delta);
}


bool ASPlayerState::RemoveCredits(int32 Delta)
{
	if (Delta <= 0) {
		return false;
	}

	if (Credits < Delta) {
		return false;
	}

	Credits -= Delta;
	OnCreditsChanged.Broadcast(this, Credits, -Delta);
	return true;
}

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}

void ASPlayerState::GetSaveGameData(USSaveGame* SaveGameObject)
{
	if (SaveGameObject) {

		AddCredits(SaveGameObject->Credits);
	}
}

void ASPlayerState::SetSaveGameData(USSaveGame* SaveGameObject)
{
	if (SaveGameObject) {

		SaveGameObject->Credits = GetCredits();
	}
}
