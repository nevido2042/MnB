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

	virtual void SetupInputComponent() override;

	void ClickMove();

	FVector GetClickLocation();
};
