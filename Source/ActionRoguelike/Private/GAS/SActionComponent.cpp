#include "GAS/SActionComponent.h"
#include "Net/UnrealNetwork.h"
#include "../ActionRoguelike.h"
#include "Engine/ActorChannel.h"


USActionComponent::USActionComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void USActionComponent::InitAction()
{
	for (TSubclassOf<USAction> DefaultAction : DefaultActions)
	{
		AddAction(GetOwner(),DefaultAction);
	}
}

bool USActionComponent::RemoveAction(USAction* RemoveAction)
{
	if (RemoveAction && !RemoveAction->IsRunning()) {
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
	//服务器才可以给buff
	if (!Starter->HasAuthority()) {
		UE_LOG(LogTemp,Warning,TEXT("Client Can not Add Action"))
		return;
	}

	USAction * Action= NewObject<USAction>(GetOwner(),NewAction);

	if (Action) {
		Action->InitialOwerComp(this);
		Actions.Add(Action);
		//可以添加即开始能力，并且可以激活能力
		if (Action->isAutoStart && Action->CanStart(Starter)) {
			
			Action->StartAction(Starter); //需要参数，实施效果的Action
		}
	}
}

bool USActionComponent::StartActionForName(AActor* Starter, FName ActionName)
{
	for (USAction* Action : Actions ) {
		if (Action && Action->ActionName == ActionName) {
			if (Action->CanStart(Starter)) { //标签符合才执行
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
			if (Action->IsRunning()) { //在运行中才结束
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




void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//调试标签
	//FString DebugMsg = GetNameSafe(GetOwner())+ ":" +ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	//调试Action
	for (USAction* Action:Actions) {

		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		FString ActionMsg = FString::Printf(TEXT("[%s] Action:%s IsRunning:%s Outer:%s")
			, *GetNameSafe(GetOwner())
			, *Action->ActionName.ToString()
			, Action->IsRunning() ? TEXT("true") : TEXT("false")
			, *GetNameSafe(Action->GetOuter())
			);
		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}

}

void USActionComponent::BeginPlay()
{
	if (GetOwner()->HasAuthority()) {
		InitAction();
	}	
}

USAction* USActionComponent::GetAction(TSubclassOf<USAction> FindActionClass)
{
	if (FindActionClass) {

		for ( USAction* Action :Actions) {
			//找到
			if (Action && Action->IsA(FindActionClass)) {

				return Action;
			}
		}
	}
	//没找到
	return nullptr;
}

bool USActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	//组件Uobject复制
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (USAction* Action :Actions) {
		if (Action) {
			bWroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	return bWroteSomething;
}

void USActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, Actions);
}