// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "MyGameModeBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100;
	Health = MaxHealth;
	// 支持同步
	SetIsReplicatedByDefault(true);
}


bool USAttributeComponent::ApplyChangeHealth (AActor* Attack,float Val)
{
	//god 模式不可以被伤害

	if (Val< 0 && !GetOwner()->CanBeDamaged()) { return false; };

	float OldHealth = Health;
	Health = FMath::Clamp<float>(Health+Val,0,MaxHealth);
	float ChangeVal = Health - OldHealth;
	ApplyHealthChange.Broadcast(Attack, this, Health, ChangeVal);

	if (ChangeVal !=0) {	//不判断死亡Actor会影响UI
		NetMulticastApplyHealthChange(Attack, Health, ChangeVal);
	}
	
	//针对死亡Actor操作
	if (Health == 0 && ChangeVal < 0) {
		AMyGameModeBase* GM = GetWorld()->GetAuthGameMode<AMyGameModeBase>();
		if (GM) {
			GM->OnActorKiller(GetOwner(),Attack);
		}
	}
	return ChangeVal != 0;
}

bool USAttributeComponent::ApplyChangeRage(AActor* Attack, float Val)
{
	if (Val!=0) {
		
		float OldRage = Rage;
		Rage = FMath::Clamp<float>(Rage + Val, 0, MaxRage);
		float ChangeVal = Rage - OldRage;
		ApplyRageChange.Broadcast(Attack, this, Rage , ChangeVal);
		return true;
	}

	return false;

}

void USAttributeComponent::NetMulticastApplyHealthChange_Implementation(AActor* Attacker, float NewHealth, float ChangeVal)
{
	ApplyHealthChange.Broadcast(Attacker,this,NewHealth,ChangeVal);
}


bool USAttributeComponent::Kill(AActor* Attack)
{
	return ApplyChangeHealth(Attack, -GetMaxHealth());
}


bool USAttributeComponent::IsAlive()
{
	return Health > 0;
}


bool USAttributeComponent::IsFullHealth()
{
	return MaxHealth <= Health;
}


USAttributeComponent* USAttributeComponent::GetAttribute(AActor* Actor)
{
	if (Actor) {
		return Cast<USAttributeComponent>(Actor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	return nullptr;
}


float USAttributeComponent::GetMaxHealth()
{
	return MaxHealth;
}

float USAttributeComponent::GetHealth()
{
	return Health;
}

float USAttributeComponent::GetRage()
{
	return Rage;
}

float USAttributeComponent::GetMaxRage()
{
	return MaxRage;
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, MaxHealth);
}