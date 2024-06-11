// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/VRInfoWidget.h"
#include "Components/TextBlock.h"

void UVRInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	UI_ActorInfo = Cast<UTextBlock>(GetWidgetFromName(TEXT("ActorInfo")));

	//UI_ActorInfo->SetText(FText::FromString("HI"));
	//SetActorInfoText(FText::FromString("HI"));
}

void UVRInfoWidget::SetActorInfoText(FText Info)
{
	UI_ActorInfo->SetText(Info);
}
