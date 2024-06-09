// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RamdomAction.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UBTTask_RamdomAction : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_RamdomAction();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	enum class Action: int
	{
		LeftAttack = 0,
		RightAttack,

		LeftGuard,
		RightGuard
	};

	Action RandomAction;

	void SetRandomAction();
	void ExecuteAction(UBehaviorTreeComponent& OwnerComp);
};
