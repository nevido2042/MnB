// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_TargetDieCheck.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UBTService_TargetDieCheck : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_TargetDieCheck();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
