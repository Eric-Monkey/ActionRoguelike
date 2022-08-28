// Fill out your copyright notice in the Description page of Project Settings.


#include "SHitFlashItem.h"
#include "SAttributeComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASHitFlashItem::ASHitFlashItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	RootComponent = StaticMeshComp;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	AttributeComp->ApplyHealthChange.AddDynamic(this,&ASHitFlashItem::HealthChange);
}

void ASHitFlashItem::HealthChange(AActor* Attacker, USAttributeComponent* AttributeComponent, float health, float ChangeVal)
{
	if (ChangeVal < 0) {

		StaticMeshComp->SetScalarParameterValueOnMaterials("GameTime", GetWorld()->GetTimeSeconds());
		UE_LOG(LogTemp,Warning,TEXT("Call____HealthChange"))
	}
}

