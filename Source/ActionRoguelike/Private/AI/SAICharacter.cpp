// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIController.h"
#include "SCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//AI感知组件
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComponent");
}

// Called when the game starts or when spawned
void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
}

void ASAICharacter::OnTarPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	AAIController* AIPc = Cast<AAIController>(GetController()); //获取ai控制器
	if (ensure(AIPc)) {
		ASCharacter* UpdateCharacter = Cast<ASCharacter>(Actor);
		if (UpdateCharacter) {
			AIPc->GetBlackboardComponent()->SetValueAsObject("ToActor", Actor);//将值放入黑板
		}
	};
}

