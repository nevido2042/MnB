// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PCWidget.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UPCWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UImage* LeftImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UImage* RightImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UProgressBar* UI_HealthBar;

protected:
	virtual void NativeOnInitialized();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	void UpdateDirection(class AMnBCharacter* MnBCharacter);
	void UpdateHealthBar(class UHealth * Health);
};
