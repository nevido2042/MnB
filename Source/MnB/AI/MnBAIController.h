// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MnBAIController.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API AMnBAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	explicit AMnBAIController(FObjectInitializer const& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
