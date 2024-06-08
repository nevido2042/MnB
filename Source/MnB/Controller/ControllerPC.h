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
	
};
