// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
class USizeBox;
UCLASS()
class ACTIONROGUELIKE_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;

	UPROPERTY(EditAnywhere,Category="AI")
	FVector WordOffset;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="UI")
	AActor* AttachTo;

	UPROPERTY(EditAnywhere,Category="WolrdUI")
	TSubclassOf<UUserWidget> UI;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
};
