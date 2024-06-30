// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_SearchTarget.h"

#include "AI/MnBAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SearchTarget::UBTTask_SearchTarget(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Search Target");
}

EBTNodeResult::Type UBTTask_SearchTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Cont = Cast<AMnBAIController>(OwnerComp.GetAIOwner()))
	{
		Cont->SearchTarget();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
