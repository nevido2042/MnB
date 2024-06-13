// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "MyEngine.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UMyEngine : public UEngine
{
	GENERATED_BODY()
	
public:
	UMyEngine();

	virtual bool InitializeHMDDevice() override;

	void SetStartInVR(bool bValue);
};
