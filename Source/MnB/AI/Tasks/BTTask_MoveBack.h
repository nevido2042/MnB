// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveBack.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UBTTask_MoveBack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	explicit UBTTask_MoveBack(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
