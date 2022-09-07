// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractComponent.h"
#include "SInterface.h"
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<bool> bDebugDraw(TEXT("su.DebugDraw"),false,TEXT("Enable bDebugDraw"),ECVF_Cheat);
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

	TArray<FHitResult> HitResults;
	FVector Beg;
	FVector End;
	FRotator PawnEyeRot;
	AActor* MyPOwner = GetOwner();
	APawn* MyPawn = Cast<APawn>(MyPOwner);
	Beg = MyPawn->GetPawnViewLocation();
	PawnEyeRot = MyPawn->GetViewRotation();
	End = Beg + PawnEyeRot.Vector() * 500;

	FCollisionShape Shape;
	radius = 20.f;
	Shape.SetSphere(radius);

	//bool IsHit = GetWorld()->LineTraceSingleByObjectType(HitResult,Beg,End, CollisionObjQuePar);
	bool IsHit = GetWorld()->SweepMultiByObjectType(HitResults,Beg,End, FQuat::Identity, CollisionObjQuePar,Shape);
	
	if (IsHit) {
		for (FHitResult& HitResult : HitResults) {
			//≤‚ ‘√¸÷–∑∂Œß
			if (bDebugDraw.GetValueOnGameThread()) {
				DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, radius, 32, FColor::Green, false, 2.0f);
				
			}
		
			AActor* HitActor = HitResult.GetActor();
			if (HitActor->Implements<USInterface>()) {
				ISInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}		
	}
	//≤‚ ‘…‰œﬂ
	if (bDebugDraw.GetValueOnGameThread()) {
		FColor col = IsHit ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), Beg, End, col, true, 2, 0, 2);
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

