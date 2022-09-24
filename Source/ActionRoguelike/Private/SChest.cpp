// Fill out your copyright notice in the Description page of Project Settings.


#include "SChest.h"
#include "Components/StaticMeshComponent.h"
#include "GAS/SActionComponent.h"
#include "Net/UnrealNetwork.h"

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
	bChestOpen = false;
	SetReplicates(true);
}


void ASChest::Interact_Implementation(APawn* CallPawn) {
	//测试同步，注释前置条件
	/*USActionComponent* ActionComp = Cast<USActionComponent>(CallPawn->GetComponentByClass(USActionComponent::StaticClass()));
	if (ActionComp && ActionComp->ActiveGameplayTags.HasAnyExact(CardTags)) {

		LipMesh->SetRelativeRotation(FRotator(LidRotationForX, 0, 0));
	}		*/

	bChestOpen = !bChestOpen;
	float CurrentPitch = bChestOpen ? LidRotationForX : 0;
	LipMesh->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));
}

void ASChest::OnRep_ChestOpen()
{
	float CurrentPitch = bChestOpen ? LidRotationForX : 9;
	LipMesh->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));
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

void ASChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASChest,bChestOpen);
}

