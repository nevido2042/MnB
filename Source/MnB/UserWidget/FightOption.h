// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FightOption.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UFightOption : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* AttackBtn = nullptr;

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void LoadFightLevel();
};
