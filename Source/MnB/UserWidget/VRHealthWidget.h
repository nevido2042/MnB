// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VRHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UVRHealthWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UProgressBar* UI_HeathBar;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetHealthBar(float Value);
};
