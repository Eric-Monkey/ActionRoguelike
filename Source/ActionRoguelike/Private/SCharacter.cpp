// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LatentActions.h"
#include "SBaseProjectile.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;


	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	InteractComp = CreateDefaultSubobject<USInteractComponent>("InteractComp");
	IsAttack = false;
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

void ASCharacter::PlayAttackAnim() {
	if (AttackMontage && !IsAttack) {
		IsAttack = true;
		float Dely = PlayAnimMontage(AttackMontage,1);
		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle,this, &ASCharacter::SetIsAttack,Dely/3,true);
	}
}

void ASCharacter::CreateProjectile(TSubclassOf<AActor> SpawnProjectile)
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	
	FCollisionObjectQueryParams ObjQueryParams;
	ObjQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	//���ú���Actor����
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	//����ɨ����״
	FCollisionShape Shape;
	Shape.SetSphere(20);

	//���н��
	FHitResult HitResult;

	//��ʼλ��
	FVector Beg = CameraComp->GetComponentLocation();
	//����λ��
	FVector End = CameraComp->GetComponentRotation().Vector() * 5000 + Beg;

	//���߼��
	GetWorld()->SweepSingleByObjectType(HitResult, Beg, End, FQuat::Identity, ObjQueryParams, Shape, QueryParams);
	FString Text = FString::Printf(TEXT("Hit at location:%s___ %s"), *HitResult.ImpactPoint.ToString(),*GetDebugName(HitResult.GetActor()));
	DrawDebugString(GetWorld(), HitResult.ImpactPoint, Text, nullptr, FColor::Green, 2, true, 1);
	bool IsHit = HitResult.bBlockingHit;
	
	if (IsHit) {
		
		FVector HitPoint = HitResult.ImpactPoint;	//��ȡ���е�
		FRotator FProjectileRot = FRotationMatrix::MakeFromX(HitPoint - HandLocation).Rotator();
		FTransform SpawnTM = FTransform(FProjectileRot, HandLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;		//����������

		GetWorld()->SpawnActor<AActor>(SpawnProjectile, SpawnTM, SpawnParams);	
	}
	else
	{
		FRotator FProjectileRot = FRotationMatrix::MakeFromX(End - HandLocation).Rotator();
		FTransform SpawnTM = FTransform(FProjectileRot, HandLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;		//����������

		GetWorld()->SpawnActor<AActor>(SpawnProjectile, SpawnTM, SpawnParams);
	}
	/*FColor col = IsHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), Beg, End, col, true, 2, 0, 2);*/
	
	FColor col = IsHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), Beg, End, col, true, 2, 0, 2);
}

void ASCharacter::UseBaseProjectile()
{
	Projectile = ASBaseProjectile().GetClass();
}

void ASCharacter::UseTeleProjectile()
{
	//Projectile = TeleProjectile;
}

void ASCharacter::UseGProjectile()
{
	//Projectile = GProjectile;
}

//���Ƶ��һֱ������̫��
void ASCharacter::SetIsAttack() {
	IsAttack = false;
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
}
void ASCharacter::Attack() {
	CreateProjectile(Projectile);	
}

void ASCharacter::PrimaryInteract() {
	if (ensure(InteractComp))
	InteractComp->PrimaryInteract();
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward",this,&ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("TurnUp",this,&APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRight",this,&APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ASCharacter::PlayAttackAnim);
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("Interact",IE_Pressed,this, &ASCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("UseBaseProjectile", IE_Pressed, this, &ASCharacter::UseBaseProjectile);
	PlayerInputComponent->BindAction("UseTeleProjectile", IE_Pressed, this, &ASCharacter::UseTeleProjectile);
	PlayerInputComponent->BindAction("UseGProjectile",IE_Pressed,this, &ASCharacter::UseGProjectile);
}
