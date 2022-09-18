#include "GAS/SActionComponent.h"


USActionComponent::USActionComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

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
		//������Ӽ���ʼ���������ҿ��Լ�������
		if (Action->isAutoStart && Action->CanStart()) {
			
			Action->StartAction(Starter); //��Ҫ������ʵʩЧ����Action
		}
	}
}

bool USActionComponent::StartActionForName(AActor* Starter, FName ActionName)
{
	for (USAction* Action : Actions ) {
		if (Action && Action->ActionName == ActionName) {
			if (Action->CanStart()) { //��ǩ���ϲ�ִ��
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
			if (Action->IsRuning()) { //�������вŽ���
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

