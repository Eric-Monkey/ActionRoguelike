// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, PlayState, int32, NewCredits, int32, Delta);



class USSaveGame;
/**
*
*/
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	int32 Credits;

public:
	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const;

	UPROPERTY(BlueprintAssignable,Category = "Events")
	FOnCreditsChanged OnCreditsChanged;

	UFUNCTION()
	void SetSaveGameData(USSaveGame* SaveGameObject);

	UFUNCTION()
	void GetSaveGameData(USSaveGame* SaveGameObject);
};
