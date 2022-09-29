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
	//�������ſ��Ը�buff
	if (!Starter->HasAuthority()) {
		UE_LOG(LogTemp,Warning,TEXT("Client Can not Add Action"))
		return;
	}

	USAction * Action= NewObject<USAction>(GetOwner(),NewAction);

	if (Action) {
		Action->InitialOwerComp(this);
		Actions.Add(Action);
		//������Ӽ���ʼ���������ҿ��Լ�������
		if (Action->isAutoStart && Action->CanStart(Starter)) {
			
			Action->StartAction(Starter); //��Ҫ������ʵʩЧ����Action
		}
	}
}

bool USActionComponent::StartActionForName(AActor* Starter, FName ActionName)
{
	for (USAction* Action : Actions ) {
		if (Action && Action->ActionName == ActionName) {
			if (Action->CanStart(Starter)) { //��ǩ���ϲ�ִ��
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
			if (Action->IsRunning()) { //�������вŽ���
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
	//���Ա�ǩ
	//FString DebugMsg = GetNameSafe(GetOwner())+ ":" +ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	//����Action
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
			//�ҵ�
			if (Action && Action->IsA(FindActionClass)) {

				return Action;
			}
		}
	}
	//û�ҵ�
	return nullptr;
}

bool USActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	//���Uobject����
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