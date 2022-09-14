// Fill out your copyright notice in the Description page of Project Settings.


#include "SChest.h"
#include "Components/StaticMeshComponent.h"
#include "GAS/SActionComponent.h"

// Sets default values
ASChest::ASChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LipMesh = CreateDefaultSubobject<UStaticMeshComponent>("LipMesh");
	LipMesh->SetupAttachment(BaseMesh);

	LidRotationForX = 110;
}


void ASChest::Interact_Implementation(APawn* CallPawn) {
	UE_LOG(LogTemp, Log, TEXT("Call Open Chest"));
	USActionComponent* ActionComp = Cast<USActionComponent>(CallPawn->GetComponentByClass(USActionComponent::StaticClass()));
	UE_LOG(LogTemp, Log, TEXT("Call Open Chest1"));
	if (ActionComp) {
		UE_LOG(LogTemp, Log, TEXT("Call Open Chest2"));
	}
	if (ActionComp && ActionComp->ActiveGameplayTags.HasAnyExact(CardTags)) {

		UE_LOG(LogTemp, Log, TEXT("Open Chest"));
		LipMesh->SetRelativeRotation(FRotator(LidRotationForX, 0, 0));
	}
		//LipMesh->SetRelativeRotation(FRotator(LidRotationForX, 0, 0));
}

// Called when the game starts or when spawned
void ASChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

