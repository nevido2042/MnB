// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldOption.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UWorldOption : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* EnterTheTown = nullptr;

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void LoadTownLevel();
	
};
