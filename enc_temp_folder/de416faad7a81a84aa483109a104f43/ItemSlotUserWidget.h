// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotUserWidget.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnInvenButtonEvent, UItemSlotUserWidget* /*Widget*/);

UCLASS()
class MNB_API UItemSlotUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UImage* Image = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* Button = nullptr;

	uint32 ItemIndex = (uint32)-1;
protected:

	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnItemBtnHovered();
	UFUNCTION()
	void OnItemBtnClicked();
	UFUNCTION()
	void OnItemBtnReleased();

public:
	void SetImage(UTexture2D* InImage);

	FOnInvenButtonEvent ItemBtnHovered;
	FOnInvenButtonEvent ItemBtnClicked;
	FOnInvenButtonEvent ItemBtnReleased;
};
