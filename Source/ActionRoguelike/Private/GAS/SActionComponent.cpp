#include "GAS/SActionComponent.h"


USActionComponent::USActionComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}


void USActionComponent::AddAction(TSubclassOf<USAction> NewAction)
{
	if ( !NewAction ) {
		return;
	}

	USAction * Action= NewObject<USAction>(this,NewAction);
	if (Action) {
		Actions.Add(Action);
	}
}

bool USActionComponent::StartActionForName(AActor* Starter, FName ActionName)
{
	for (USAction* Action : Actions ) {
		if (Action && Action->ActionName == ActionName) {
			if (Action->CanStart(Starter)) { //标签符合才执行
				Action->StartAction(Starter);
				return true;
			}
		}
	}
	return false;
}

bool USActionComponent::EndActionForName(AActor* Starter, FName ActionName)
{
	for (USAction* Action :Actions) {
		if (Action && Action->ActionName == ActionName){
			if (Action->IsRuning()) { //不在运行中才结束
				Action->EndAction(Starter);
				return true;
			}	
		}
	}
	return false;
}

void USActionComponent::InitAction()
{

	for (TSubclassOf<USAction> DefaultAction : DefaultActions)
	{
		AddAction(DefaultAction);
	}
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
	FString DebugMsg = GetNameSafe(GetOwner())+ ":" +ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

