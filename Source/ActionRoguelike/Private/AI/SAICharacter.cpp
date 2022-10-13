// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIController.h"
#include "SCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "UI/MyUserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/SActionComponent.h"

// Sets default value s

ASAICharacter::ASAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//AI感知组件
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComponent");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//Attribute 组件
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	FlashSpeed = 5;

	//ActionComp 组件
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
}

bool ASAICharacter::IsAlive()
{
	if ensure(AttributeComp) {
		return AttributeComp->IsAlive();
	}	
	return false;
}

void ASAICharacter::OnHealthChange(AActor* Attacker, USAttributeComponent* AttributeComponent, float health, float ChangeVal)
{
	//
	if (ChangeVal < 0) {
		
		if (HealthBar == nullptr) {
			HealthBar = CreateWidget<UMyUserWidget>(GetWorld(), UMG_HealthBar);
			if (HealthBar) {
				HealthBar->AttachTo = this;
				
				HealthBar->AddToViewport(0);
			}
		}


		GetMesh()->SetScalarParameterValueOnMaterials("HitGameTime", GetWorld()->GetTimeSeconds());//被击闪光
		GetMesh()->SetScalarParameterValueOnMaterials("FlashSpeed", FlashSpeed);//闪光事件
		AAIController* AIPc = Cast<AAIController>(GetController()); //获取ai控制器
		if (ensure(AIPc) && Attacker!=this) {
			AIPc->GetBlackboardComponent()->SetValueAsObject("ToActor", Attacker);//将值放入黑板
		};
	}


	if ( health <= 0) {
		AAIController* AIPc = Cast<AAIController>(GetController());
		if (ensure(AIPc)) {
			AIPc->GetBrainComponent()->StopLogic("Killer"); //停止行为树
				
			GetMesh()->SetAllBodiesSimulatePhysics(true);											//set ragdoll
			GetMesh()->SetCollisionProfileName("Ragdoll");
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			
			SetLifeSpan(10);//set lifespan
		}
	}
	
}

// Called when the game starts or when spawned
void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASAICharacter::AddSpotWidgetToView()
{
	
	UMyUserWidget* SpotWidget = CreateWidget<UMyUserWidget>(GetWorld(),SpottedWidgetClass);
	if (SpotWidget) {
		SpotWidget->AttachTo = this;

		SpotWidget->AddToViewport(10);
	}
}

// Called every frame
void ASAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,&ASAICharacter::OnTarPerceptionUpdate);

	AttributeComp->ApplyHealthChange.AddDynamic(this, &ASAICharacter::OnHealthChange);
}


void ASAICharacter::OnTarPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{	
	if (Stimulus.IsActive()) {
		AAIController* AIPc = Cast<AAIController>(GetController()); //获取ai控制器
		if (!ensure(AIPc)) {
			return;
		};
		//新目标弹出ui
		if (GetTargetActor(AIPc)!= Actor) {
			AddSpotWidgetToView();
		}

		if (Cast<ASCharacter>(Actor)) {

			AIPc->GetBlackboardComponent()->SetValueAsObject("ToActor", Actor);//将值放入黑板

		}
	}		
}

AActor* ASAICharacter::GetTargetActor(AAIController* AIPC)
{
	return Cast<AActor>(AIPC->GetBlackboardComponent()->GetValueAsObject("ToActor"));
}

