// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ControllerPC.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API AControllerPC : public APlayerController
{
	GENERATED_BODY()

public:
	AControllerPC();

protected:
	virtual void BeginPlay() override;

	TSubclassOf<class UUserWidget> Widget = nullptr;

	UUserWidget* CurrentWidget = nullptr;
	
public:
	UUserWidget* GetCurrentWidget() { return CurrentWidget; }

protected:
	virtual void OnPossess(APawn* aPawn) override;

public:
	void PickUpItem(const FName& InKey);

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UInventoryUserWidget> InventoryWidgetAsset = nullptr;

	UInventoryUserWidget* InventoryWidget = nullptr;

	void CreateInventoryWidget();

};
