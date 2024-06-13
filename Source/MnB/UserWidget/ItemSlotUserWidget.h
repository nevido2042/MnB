// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UItemSlotUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UImage* Image = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* Button = nullptr;

protected:

	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

public:
	void SetImage(UTexture2D* InImage);
};
