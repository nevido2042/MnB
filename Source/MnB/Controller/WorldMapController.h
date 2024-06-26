// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WorldMapController.generated.h"

/**
 * 
 */

class UInputAction;
class UInputMappingContext;

UCLASS()
class MNB_API AWorldMapController : public APlayerController
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds);

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ClickMoveAction;

	/** Click Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ClickAction;

	virtual void SetupInputComponent() override;

	void ClickMove();

	void Click();

	FVector GetClickLocation();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WorldOptionAsset = nullptr;

	UUserWidget* WorldOption = nullptr;
};
