#include "GAS/SActionComponent.h"
#include "Net/UnrealNetwork.h"


USActionComponent::USActionComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


bool USActionComponent::RemoveAction(USAction* RemoveAction)
{
	if (RemoveAction && !RemoveAction->IsRuning()) {
		Actions.Remove(RemoveAction);
		return true;
	}
	return false;
}
	

void USActionComponent::AddAction(AActor* Starter, TSubclassOf<USAction> NewAction)
{
	if ( !NewAction ) {
		return;
	}

	USAction * Action= NewObject<USAction>(this,NewAction);
	if (Action) {
		Actions.Add(Action);
		//可以添加即开始能力，并且可以激活能力
		if (Action->isAutoStart && Action->CanStart()) {
			
			Action->StartAction(Starter); //需要参数，实施效果的Action
		}
	}
}

bool USActionComponent::StartActionForName(AActor* Starter, FName ActionName)
{
	for (USAction* Action : Actions ) {
		if (Action && Action->ActionName == ActionName) {
			if (Action->CanStart()) { //标签符合才执行
				// if is client ,call server startAction
				if (! GetOwner()->HasAuthority()) {
					ServerStartActionForName(Starter, ActionName);
				}
				
				Action->StartAction(Starter);
				return true;
			}
		}
	}
	return false;
}

//Server_RPC_StartActionComp
void USActionComponent::ServerStartActionForName_Implementation(AActor* Starter, FName ActionName)
{
	StartActionForName(Starter,ActionName);

}

bool USActionComponent::EndActionForName(AActor* Starter, FName ActionName)
{
	for (USAction* Action :Actions) {
		if (Action && Action->ActionName == ActionName){
			if (Action->IsRuning()) { //在运行中才结束
				//if is client
				if (! GetOwner()->HasAuthority()) {
					ServerEndActionForName(Starter,ActionName);
				}
				Action->EndAction(Starter);
				return true;
			}	
		}
	}
	return false;
}

void USActionComponent::ServerEndActionForName_Implementation(AActor* Starter, FName ActionName)
{
	EndActionForName(Starter,ActionName);
}


void USActionComponent::InitAction()
{

	for (TSubclassOf<USAction> DefaultAction : DefaultActions)
	{
		AddAction(GetOwner(),DefaultAction);
	}
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
	FString DebugMsg = GetNameSafe(GetOwner())+ ":" +ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

void USActionComponent::BeginPlay()
{
	InitAction();
}

