// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WorldSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UWorldSaveGame : public USaveGame
{
	GENERATED_BODY()

public: // Player
	UPROPERTY()
	FVector PlayerLocation;

	UPROPERTY()
	FRotator PlayerRotation;

	
};
