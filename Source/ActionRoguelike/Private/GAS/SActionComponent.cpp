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

	USAction * Action= NewObject<USAction>(GetOuter(),NewAction);
	if (Action) {
		Actions.Add(Action);
	}
}

bool USActionComponent::StartActionForName(AActor* Starter, FName ActionName)
{
	for (USAction* Action : Actions ) {
		if (Action && Action->ActionName == ActionName) {
			Action->StartAction(Starter);
			return true;
		}
	}
	return false;
}

bool USActionComponent::EndActionForName(AActor* Starter, FName ActionName)
{
	for (USAction* Action:Actions) {
		if (Action && Action->ActionName == ActionName){
			Action->EndAction(Starter);
			return true;
		}
	}
	return false;
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

