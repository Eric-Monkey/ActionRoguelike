// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "MyGameModeBase.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100;
	Health = MaxHealth;
	// ...
}


USAttributeComponent* USAttributeComponent::GetAttribute(AActor* Actor)
{
	if (Actor) {
		return Cast<USAttributeComponent>(Actor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool USAttributeComponent::IsAlive()
{
	return Health>0;
}

bool USAttributeComponent::IsFullHealth()
{
	return MaxHealth <= Health;
}

float USAttributeComponent::GetMaxHealth()
{
	return MaxHealth;
}

float USAttributeComponent::GetHealth()
{
	return Health;
}

bool USAttributeComponent::ApplyChangeHealth (AActor* Attack,float Val)
{
	if (!GetOwner()->CanBeDamaged()) { return false; };

	float OldHealth = Health;
	Health = FMath::Clamp<float>(Health+Val,0,MaxHealth);
	float ChangeVal = Health - OldHealth;
	ApplyHealthChange.Broadcast(Attack,this, Health, ChangeVal);

	//’Î∂‘À¿ÕˆActor≤Ÿ◊˜
	if (Health == 0 && ChangeVal < 0) {
		AMyGameModeBase* GM = GetWorld()->GetAuthGameMode<AMyGameModeBase>();
		if (GM) {
			GM->OnActorKiller(GetOwner(), Attack);
		}
	}
	return ChangeVal != 0;
}

bool USAttributeComponent::Kill(AActor* Attack)
{
	return ApplyChangeHealth(Attack, -GetMaxHealth());
}



