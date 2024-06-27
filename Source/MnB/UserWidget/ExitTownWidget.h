// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExitTownWidget.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UExitTownWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UProgressBar* ExitTownBar = nullptr;

	virtual void NativeOnInitialized() override;

	float CurPercent = 0.f;

public:
	UProgressBar* GetExitTownBar() { return ExitTownBar; }

	float GetCurPercent() { return CurPercent; }
	void SetCurPercent(float Value) { CurPercent = Value; }

	
};
