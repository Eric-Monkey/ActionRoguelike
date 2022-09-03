// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/SAICharacter.h"
#include "Math/RotationMatrix.h"

USBTTaskNode::USBTTaskNode() {
	
	RandomOffset = 2;//AI射击瞄准偏移
	
}

EBTNodeResult::Type USBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp)) {
		APawn*  ToActor= Cast<APawn>(BlackboardComp->GetValueAsObject("ToActor"));//获取黑板中目标 Actor
		
		if (!ToActor) return EBTNodeResult::Failed;
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttribute(ToActor);

		//获取玩家组件，并判断玩家是否死亡，如果死亡则任务失败
		if (!ensure(AttributeComp)) return EBTNodeResult::Failed; 
		if(! AttributeComp->IsAlive()) return EBTNodeResult::Failed;
		
		AAIController* AIPc = Cast<AAIController>(OwnerComp.GetOwner());		
		if ( ensure(AIPc) ) {
			APawn* AIPawn = AIPc->GetPawn();
			if (AIPawn) {
				ASAICharacter* AICharacter= Cast<ASAICharacter>(AIPawn); //获得AI 角色
				FVector  AIAttactLocBeg= AICharacter->GetMesh()->GetSocketLocation("Muzzle_Front");
				FVector  End = ToActor->GetActorLocation();

				FRotator MuzzleRot = FRotationMatrix::MakeFromX(End - AIAttactLocBeg).Rotator();
				//AI准度加偏移，不是百分百击中
				MuzzleRot.Pitch += FMath::RandRange(0.0f, RandomOffset); 
				MuzzleRot.Yaw += FMath::RandRange(-RandomOffset, RandomOffset);
				

				FTransform SpawnTM = FTransform(MuzzleRot, AIAttactLocBeg);

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Instigator = AICharacter;		//设置生成者

				GetWorld()->SpawnActor<ASBaseProjectile>(ProjectileEffect,SpawnTM,SpawnParams);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
