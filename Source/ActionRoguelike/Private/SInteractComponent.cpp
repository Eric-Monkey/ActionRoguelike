// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractComponent.h"
#include "SInterface.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
USInteractComponent::USInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USInteractComponent::PrimaryInteract() {
	FCollisionObjectQueryParams CollisionObjQuePar;
	CollisionObjQuePar.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

	FHitResult HitResult;
	FVector Beg;
	FVector End;
	FRotator PawnEyeRot;
	AActor* MyPOwner = GetOwner();
	APawn* MyPawn = Cast<APawn>(MyPOwner);
	Beg = MyPawn->GetPawnViewLocation();
	PawnEyeRot = MyPawn->GetViewRotation();
	End = Beg + PawnEyeRot.Vector() * 500;
	bool IsHit = GetWorld()->LineTraceSingleByObjectType(HitResult,Beg,End, CollisionObjQuePar);

	if (IsHit) {
		AActor* HitActor= HitResult.GetActor();
		if (HitActor->Implements<USInterface>()) {
			ISInterface::Execute_Interact(HitActor,MyPawn);
		}
	}

}

// Called when the game starts
void USInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

