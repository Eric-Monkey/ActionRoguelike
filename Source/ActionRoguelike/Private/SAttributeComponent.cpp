// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100;
	Health = MaxHealth;
	// ...
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

bool USAttributeComponent::ApplyChangeHealth (float Val)
{
	float OldHealth = Health;

	Health = FMath::Clamp<float>(Health+Val,0,MaxHealth);

	float ChangeVal = Health - OldHealth;
	ApplyHealthChange.Broadcast(nullptr,this, Health, ChangeVal);

	return ChangeVal == 0;
}




