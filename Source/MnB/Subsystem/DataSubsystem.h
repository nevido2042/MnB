// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataSubsystem.generated.h"

/**
 * 
 */

struct FItemData;

UCLASS()
class MNB_API UDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UDataSubsystem();

	const FItemData* FindItem(const FName& InKey);

protected:
	UPROPERTY()
	UDataTable* ItemDataTable;
};
