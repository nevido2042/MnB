// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/PCWidget.h"
#include "Components/Image.h"
#include "MnBCharacter.h"
#include "Kismet/GameplayStatics.h"

void UPCWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	LeftImage = Cast<UImage>(GetWidgetFromName(TEXT("ImageLeft")));

	RightImage = Cast<UImage>(GetWidgetFromName(TEXT("ImageRight")));
}

void UPCWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		// �÷��̾� ĳ���� ��������
		AMnBCharacter* MnBCharacter = Cast<AMnBCharacter>(PlayerController->GetCharacter());
		if (MnBCharacter)
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
	}
}
