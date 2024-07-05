// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.generated.h"
/**
 * 
 */
USTRUCT()
struct MNB_API FItemData : public FTableRowBase
{
	GENERATED_BODY()
public:
	FItemData();

public:
	UPROPERTY(EditAnywhere)
	FName ItemName;

	UPROPERTY(EditAnywhere)
	UTexture2D* ItemImage;

	UPROPERTY(EditAnywhere)
	UMaterial* Material;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UItem> ItemFunctionClass;

//public:
//	void UseItem();
};
