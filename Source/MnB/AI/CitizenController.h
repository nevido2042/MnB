// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/MnBAIController.h"
#include "CitizenController.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API ACitizenController : public AMnBAIController
{
	GENERATED_BODY()
	
	virtual void Tick(float DeltaTime) override;
};
