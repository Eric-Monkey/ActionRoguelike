// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

static void LogOnScreen(UObject* WorldContext, FString Msg, FColor color = FColor::White, float Duration = 5.0f)
{
	if (!ensure(WorldContext)){
		return;
	}

	UWorld* World = WorldContext->GetWorld();
	FString NetPrefix = World->IsNetMode(NM_Client) ? "[Client] " : "[Server] ";

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, Duration, color, NetPrefix + Msg);
	}
}