// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

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

int32 ASPlayerState::GetCredits(int32 Delta) const
{
	return Credits;
}
