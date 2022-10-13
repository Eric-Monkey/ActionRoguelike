// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LatentActions.h"
#include "DrawDebugHelpers.h"
#include "SBaseProjectile.h"
#include "SAttributeComponent.h"
#include "GAS/SActionComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	//相机组件
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//交互组件
	InteractComp = CreateDefaultSubobject<USInteractComponent>("InteractComp");

	//属性组件
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	//能力组件
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::MoveForward(float value)
{
	FRotator ControlRot=GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;
	AddMovementInput(ControlRot.Vector(),value);
}

void ASCharacter::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, value);
}

//重构废弃
//void ASCharacter::PlayAttackAnim() {
//	if (AttackMontage && !IsAttack) {
//		IsAttack = true;
//		float Dely = PlayAnimMontage(AttackMontage,1);
//		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle,this, &ASCharacter::SetIsAttack,Dely/3,true);
//	}
//}

//重构废弃
// void ASCharacter::CreateProjectile(TSubclassOf<ASBaseProjectile> SpawnProjectile)
//{
//	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
//	
//	FCollisionObjectQueryParams ObjQueryParams;
//	ObjQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
//	ObjQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
//	ObjQueryParams.AddObjectTypesToQuery(ECC_Pawn);
//
//	//设置忽视Actor队列
//	FCollisionQueryParams QueryParams;
//	QueryParams.AddIgnoredActor(this);
//
//	//设置扫描形状
//	FCollisionShape Shape;
//	Shape.SetSphere(10);
//
//	//击中结果
//	FHitResult HitResult;
//
//	//起始位置
//	FVector Beg = CameraComp->GetComponentLocation();
//	//结束位置
//	FVector End = CameraComp->GetComponentRotation().Vector() * 5000 + Beg;
//
//	//射线检测
//	bool IsHit = GetWorld()->SweepSingleByObjectType(HitResult, Beg, End, FQuat::Identity, ObjQueryParams, Shape, QueryParams);
//	UE_LOG(LogTemp, Warning, TEXT("%d"), IsHit);
//	FString Text = FString::Printf(TEXT("Hit at location:%s___ %s"),*HitResult.ImpactPoint.ToString(), *GetDebugName(HitResult.GetActor()));
//	DrawDebugString(GetWorld(), HitResult.ImpactPoint, Text, nullptr, FColor::Green, 2, true, 1);
//	
//	if (IsHit) {
//		
//		FVector HitPoint = HitResult.ImpactPoint;	//获取击中点
//		FRotator FProjectileRot = FRotationMatrix::MakeFromX(HitPoint - HandLocation).Rotator();
//		FTransform SpawnTM = FTransform(FProjectileRot, HandLocation);
//
//		FActorSpawnParameters SpawnParams;
//		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//		SpawnParams.Instigator = this;		//设置生成者
//
//		GetWorld()->SpawnActor<ASBaseProjectile>(SpawnProjectile, SpawnTM, SpawnParams);	
//	}
//	else
//	{
//		FRotator FProjectileRot = FRotationMatrix::MakeFromX(End - HandLocation).Rotator();
//		FTransform SpawnTM = FTransform(FProjectileRot, HandLocation);
//
//		FActorSpawnParameters SpawnParams;
//		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//		SpawnParams.Instigator = this;		//设置生成者
//
//		GetWorld()->SpawnActor<ASBaseProjectile>(SpawnProjectile, SpawnTM, SpawnParams);
//	}
//	/*FColor col = IsHit ? FColor::Green : FColor::Red;
//	DrawDebugLine(GetWorld(), Beg, End, col, true, 2, 0, 2);*/
//	
//}

//限制点击一直触发蒙太奇，重构废弃
//void ASCharacter::SetIsAttack() {
//	IsAttack = false;
//	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
//}

//重构废弃
//void ASCharacter::Attack() {
//	CreateProjectile(CurProjectile);
//}


void ASCharacter::StartAction_Sprint()
{
	ActionComp->StartActionForName(this,"Sprint");
}

void ASCharacter::EndAction_Sprint()
{
	ActionComp->EndActionForName(this, "Sprint");
}

void ASCharacter::StartAction_Attack()
{
	ActionComp->StartActionForName(this,"Attack");
}

void ASCharacter::EndAction_Attack()
{
	ActionComp->EndActionForName(this,"Attack");
}


void ASCharacter::StartAction_TeleAttack()
{
	ActionComp->StartActionForName(this, "TeleAttack");
}

void ASCharacter::EndAction_TeleAttack()
{
	ActionComp->EndActionForName(this, "TeleAttack");


}void ASCharacter::StartAction_GAttack()
{
	ActionComp->StartActionForName(this, "GAttack");
}

void ASCharacter::EndAction_GAttack()
{
	ActionComp->EndActionForName(this, "GAttack");
}


void ASCharacter::StartAction_Parry() {
	ActionComp->StartActionForName(this, "Parry");
}
void ASCharacter::EndAction_Parry() {
	ActionComp->EndActionForName(this, "Parry");
}


void ASCharacter::PrimaryInteract() {
	if (ensure(InteractComp))
	InteractComp->PrimaryInteract();
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}


void ASCharacter::OnHealthChange(AActor* Attacker, USAttributeComponent* AttributeComponent, float Health, float ChangeVal)
{
	if (ChangeVal < 0) {
		//被击闪光
		GetMesh()->SetScalarParameterValueOnMaterials("HitGameTime",GetWorld()->GetTimeSeconds());
		GetMesh()->SetScalarParameterValueOnMaterials("FlashSpeed", 5);
		
		//增加怒气
		if (AttributeComponent) {
			
			AttributeComponent->ApplyChangeRage(Attacker, FMath::Abs(ChangeVal));

		}
	}
	
	//死亡
	if (Health <=0 && ChangeVal<0) {
		APlayerController* Pc = Cast<APlayerController>(GetController());
		DisableInput(Pc);
	}
}

void ASCharacter::OnRageChange(AActor* Attacker, USAttributeComponent* AttributeComponent, float Rage, float ChangeVal)
{

}

void ASCharacter::Helself(float Val)/* default = 100 */
{
	AttributeComp->ApplyChangeHealth(this, Val);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void ASCharacter::PossessedBy(AController* NewController)
//{
//	Super::PossessedBy(NewController);
//	
//}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward",this,&ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("TurnUp",this,&APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRight",this,&APawn::AddControllerYawInput);

	
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("Interact",IE_Pressed,this, &ASCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("TeleAttack", IE_Pressed, this, &ASCharacter::StartAction_TeleAttack);
	PlayerInputComponent->BindAction("TeleAttack", IE_Released, this, &ASCharacter::EndAction_TeleAttack);


	PlayerInputComponent->BindAction("GAttack",IE_Pressed,this, &ASCharacter::StartAction_GAttack);
	PlayerInputComponent->BindAction("GAttack",IE_Released,this, &ASCharacter::EndAction_GAttack);

	PlayerInputComponent->BindAction("Sprint",IE_Pressed,this,&ASCharacter::StartAction_Sprint);
	PlayerInputComponent->BindAction("Sprint",IE_Released,this,&ASCharacter::EndAction_Sprint);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ASCharacter::StartAction_Attack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &ASCharacter::EndAction_Attack);

	PlayerInputComponent->BindAction("Parry", IE_Pressed, this, &ASCharacter::StartAction_Parry);
	PlayerInputComponent->BindAction("Parry", IE_Released, this, &ASCharacter::EndAction_Parry);
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->ApplyHealthChange.AddDynamic(this,&ASCharacter::OnHealthChange);
}

