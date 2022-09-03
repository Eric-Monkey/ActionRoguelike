// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UMyUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//�п��ܴ��ڹ��ﱻ���٣����ָ��
	if ( !IsValid(AttachTo) ) {
		RemoveFromParent();
		return;
	}
	FVector2D ScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachTo->GetActorLocation()+WordOffset, ScreenPosition)) {

		float Scale= UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition = ScreenPosition / Scale;
		if (ParentSizeBox) {

			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
		
	}	
}
