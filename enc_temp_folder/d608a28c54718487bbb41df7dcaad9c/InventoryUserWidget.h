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

protected:
	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UImage* LeftImage;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UUniformGridPanel* Panel = nullptr;

	UPROPERTY(EditAnywhere)
	UUserWidget* ItemSlot = nullptr;
	

protected:
	virtual void NativeOnInitialized() override;
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
