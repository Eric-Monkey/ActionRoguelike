// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractComponent.h"
#include "SInterface.h"
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<bool> bDebugDraw(TEXT("su.DebugDraw"),false,TEXT("Enable bDebugDraw"),ECVF_Cheat);

// Sets default values for this component's properties
USInteractComponent::USInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	Traceradius = 20.f;
	TraceTypes = ECollisionChannel::ECC_WorldDynamic;
	TraceDistance = 500.f;
}


void USInteractComponent::BesterInteract()
{
	FCollisionObjectQueryParams CollisionObjQuePar;
	CollisionObjQuePar.AddObjectTypesToQuery(TraceTypes);//ECollisionChannel::ECC_WorldDynamic

	TArray<FHitResult> HitResults;
	FVector Beg;
	FVector End;
	FRotator PawnEyeRot;
	AActor* MyPOwner = GetOwner();
	APawn* MyPawn = Cast<APawn>(MyPOwner);
	Beg = MyPawn->GetPawnViewLocation();
	PawnEyeRot = MyPawn->GetViewRotation();
	End = Beg + PawnEyeRot.Vector() * TraceDistance;

	FCollisionShape Shape;
	Shape.SetSphere(Traceradius);

	//bool IsHit = GetWorld()->LineTraceSingleByObjectType(HitResult,Beg,End, CollisionObjQuePar);
	bool IsHit = GetWorld()->SweepMultiByObjectType(HitResults, Beg, End, FQuat::Identity, CollisionObjQuePar, Shape);


	//每次检测前清除聚焦对象
	FocusedActor = nullptr;

	if (IsHit) {
		for (FHitResult& HitResult : HitResults) {
			//测试命中范围
			if (bDebugDraw.GetValueOnGameThread()) {
				DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, Traceradius , 32, FColor::Green, false, 2.0f);
			}

			AActor* HitActor = HitResult.GetActor();
			if (HitActor->Implements<USInterface>()) {
				
				//将交互对象保存,做个交互UI,而不是直接调用接口
				FocusedActor = HitActor;	
				break;

			}
		}
		//如果检测到Actor显示Ui
		if (FocusedActor && InteractUIClass!=nullptr) {
			//没创建UI创建实例
			if (InteractUIInstence == nullptr) {
				InteractUIInstence = CreateWidget<UMyUserWidget>(GetWorld(),InteractUIClass);
			}
			//有实例，切换聚焦对象，判断有没有添加到视口
			if (InteractUIInstence) {

				InteractUIInstence->AttachTo = FocusedActor;

				if (! InteractUIInstence->IsInViewport()) {

					InteractUIInstence->AddToViewport();

				}	
			}
		}
		
	}
	if (!FocusedActor && InteractUIInstence) {

		InteractUIInstence->RemoveFromParent();

	}
	//测试射线
	if (bDebugDraw.GetValueOnGameThread()) {
		
		FColor col = IsHit ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), Beg, End, col, false , 0, 0,1);
	}
}


void USInteractComponent::PrimaryInteract() {
	ServerPrimaryInteract(FocusedActor);
}

//ServerRPC_PrimaryInteract
void USInteractComponent::ServerPrimaryInteract_Implementation(AActor* ClientFocused)
{
	//调用交互接口	
	if (ClientFocused == nullptr) {
		return;
	}
	APawn* MyPawn = Cast<APawn>(GetOwner());
	ISInterface::Execute_Interact(ClientFocused, MyPawn);
}


// Called when the game starts
void USInteractComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void USInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled()) {
		BesterInteract();
	}
	
}

