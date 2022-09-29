// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/SActionAttack.h"
#include "Particles/ParticleSystem.h"
#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "SBaseProjectile.h"


USActionAttack::USActionAttack()
{
	AttackSocket = "Muzzle_01";
	AttackDelayTime = 0.2f;
}

void USActionAttack::StartAction_Implementation(AActor* Starter)
{
	Super::StartAction_Implementation(Starter);
	ASCharacter* Player =Cast<ASCharacter>(Starter);
	if (Player) {
		
		Player->PlayAnimMontage(AttackMontage);
		UGameplayStatics::SpawnEmitterAttached(AttackParticle, Player->GetMesh(), AttackSocket,FVector::ZeroVector,FRotator::ZeroRotator,EAttachLocation::SnapToTarget,true);


		FTimerHandle AttackTimerHandle;
		FTimerDelegate PlayAttack;
		PlayAttack.BindUFunction(this,"AttackDelay", Player);
		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, PlayAttack, AttackDelayTime,false);
	}

}


void USActionAttack::AttackDelay(ACharacter* Starter)
{
	FVector HandLocation = Starter->GetMesh()->GetSocketLocation(AttackSocket);

	FCollisionObjectQueryParams ObjQueryParams;
	ObjQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	//���ú���Actor����
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Starter);

	//����ɨ����״
	FCollisionShape Shape;
	Shape.SetSphere(10);

	//���н��
	FHitResult HitResult;

	//��ʼλ��
	FVector Beg = Starter->GetPawnViewLocation();
	//����λ��
	FVector End = Starter->GetControlRotation().Vector() * 5000 + Beg;

	//���߼��
	bool IsHit = GetWorld()->SweepSingleByObjectType(HitResult, Beg, End, FQuat::Identity, ObjQueryParams, Shape, QueryParams);
	UE_LOG(LogTemp, Warning, TEXT("%d"), IsHit);
	FString Text = FString::Printf(TEXT("Hit at location:%s___ %s"), *HitResult.ImpactPoint.ToString(),*AActor::GetDebugName(HitResult.GetActor()));
	DrawDebugString(GetWorld(), HitResult.ImpactPoint, Text, nullptr, FColor::Green, 2, true, 1);
	
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = Starter;//����������

	//�ͻ����������ӵ�,ֱ�ӷ���
	if (!Starter->HasAuthority()) {
		return;
	}

	if (IsHit) {

		FVector HitPoint = HitResult.ImpactPoint;	//��ȡ���е�
		FRotator FProjectileRot = FRotationMatrix::MakeFromX(HitPoint - HandLocation).Rotator();
		FTransform SpawnTM = FTransform(FProjectileRot, HandLocation);
		GetWorld()->SpawnActor<ASBaseProjectile>(CurrentProjectile, SpawnTM, SpawnParams);
	}
	else
	{
		FRotator FProjectileRot = FRotationMatrix::MakeFromX(End - HandLocation).Rotator();
		FTransform SpawnTM = FTransform(FProjectileRot, HandLocation);
		GetWorld()->SpawnActor<ASBaseProjectile>(CurrentProjectile, SpawnTM, SpawnParams);
	}

}


