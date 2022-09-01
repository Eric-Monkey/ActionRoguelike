// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/SAICharacter.h"
#include "Math/RotationMatrix.h"

USBTTaskNode::USBTTaskNode() {
}

EBTNodeResult::Type USBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp)) {
		APawn*  ToActor= Cast<APawn>(BlackboardComp->GetValueAsObject("ToActor"));//��ȡ�ڰ���Ŀ�� Actor
		AAIController* AIPc = Cast<AAIController>(OwnerComp.GetOwner());
		if ( ensure(AIPc) ) {
			APawn* AIPawn = AIPc->GetPawn();
			if (AIPawn) {
				ASAICharacter* AICharacter= Cast<ASAICharacter>(AIPawn); //���AI ��ɫ
				FVector  AIAttactLocBeg= AICharacter->GetMesh()->GetSocketLocation("Muzzle_Front");
				FVector  End = ToActor->GetActorLocation();

				FRotator MuzzleRot = FRotationMatrix::MakeFromX(End - AIAttactLocBeg).Rotator();

				FTransform SpawnTM = FTransform(MuzzleRot, AIAttactLocBeg);

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Instigator = AICharacter;		//����������

				GetWorld()->SpawnActor<ASBaseProjectile>(ProjectileEffect,SpawnTM,SpawnParams);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
