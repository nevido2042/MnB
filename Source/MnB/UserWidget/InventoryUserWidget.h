// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UInventoryUserWidget : public UUserWidget
{
	GENERATED_BODY()

	friend class UInventorySubsystem;

protected:
	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UImage* LeftImage;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UUniformGridPanel* Panel = nullptr;

	UInventorySubsystem* InventorySubsystem;

	int32 InvenSize = 0;
	
	UPROPERTY()
	TArray<class UItemSlotUserWidget*> Slots;

protected:

	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	void FlushInven();
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
