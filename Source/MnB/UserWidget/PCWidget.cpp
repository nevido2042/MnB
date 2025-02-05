// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/PCWidget.h"
#include "Components/Image.h"
#include "MnBCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"
#include "Components/Health.h"
#include "MnB/UserWidget/VRInfoWidget.h"
#include "Components/TextBlock.h"

void UPCWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	LeftImage = Cast<UImage>(GetWidgetFromName(TEXT("ImageLeft")));

	RightImage = Cast<UImage>(GetWidgetFromName(TEXT("ImageRight")));

	UI_HealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));

	UI_InfoPanel = Cast<UImage>(GetWidgetFromName(TEXT("InfoPanel")));

	UI_InfoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("InfoText")));

}

void UPCWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	//Super::NativeTick(MyGeometry, InDeltaTime);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		//update arrow direction
		AMnBCharacter* MnBCharacter = Cast<AMnBCharacter>(PlayerController->GetCharacter());
		if (MnBCharacter)
		{
			UpdateDirection(MnBCharacter);

			// update heath bar
			UHealth* Health = MnBCharacter->GetComponentByClass<UHealth>();
			if (Health)
			{
				UpdateHealthBar(Health);

			}
		}
	}
}

void UPCWidget::UpdateDirection(AMnBCharacter* MnBCharacter)
{
	FLinearColor Red = FLinearColor::Red;
	FLinearColor White = FLinearColor::White;
	LeftImage->SetColorAndOpacity(White);
	RightImage->SetColorAndOpacity(White);

	FVector2D Vector2D = MnBCharacter->GetLookAxisVector();

	if (Vector2D.X < 0)
	{
		LeftImage->SetColorAndOpacity(Red);
	}
	else
	{
		RightImage->SetColorAndOpacity(Red);
	}
}

void UPCWidget::UpdateHealthBar(UHealth* Health)
{
	UI_HealthBar->SetPercent(Health->GetCurrentHP() / Health->GetMaxHP());
	//UI_HealthBar->Percent = Health->GetCurrentHP() / Health->GetMaxHP();
	//if not use SetPercent Not Running
}

void UPCWidget::HideInfo(bool b)
{
	if (b)
	{
		UI_InfoPanel->SetVisibility(ESlateVisibility::Hidden);
		UI_InfoText->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UI_InfoPanel->SetVisibility(ESlateVisibility::Visible);
		UI_InfoText->SetVisibility(ESlateVisibility::Visible);
	}

}

void UPCWidget::SetActorInfo(FText Info)
{
	UI_InfoText->SetText(Info);
}
