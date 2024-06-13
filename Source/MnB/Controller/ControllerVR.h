// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ControllerVR.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API AControllerVR : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void SetupInputComponent();
};
