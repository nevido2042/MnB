// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControlUnitWidget.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UControlUnitWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCanvasPanel* SwordManPanel = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCanvasPanel* BowManPanel = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* SwordManCount = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* BowManCount = nullptr;

	virtual void NativeOnInitialized() override;

public:
	void ActiveSwordManPanel(bool Value, uint8 Count = 0);
	void ActiveBowManPanel(bool Value, uint8 Count = 0);
	void SetSwordManCount(uint8 Count);
	void SetBowManCount(uint8 Count);

	
};
