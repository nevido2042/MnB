// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleSituationWidget.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UBattleSituationWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UProgressBar* ATeamBar = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UProgressBar* BTeamBar = nullptr;

public:
	void SetATeamBarPercent(float Value);
	void SetBTeamBarPercent(float Value);

	
};
