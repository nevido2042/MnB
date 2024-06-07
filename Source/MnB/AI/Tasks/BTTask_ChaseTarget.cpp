// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_ChaseTarget.h"

#include "AI/MnBAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ChaseTarget::UBTTask_ChaseTarget(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase Target");
}

EBTNodeResult::Type UBTTask_ChaseTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Cont = Cast<AMnBAIController>(OwnerComp.GetAIOwner()))
	{
		auto const TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

		// move to palyer location
		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(Cont, PlayerLocation);
		Cont->MoveToLocation(TargetLocation, 100.f); //wantDistace �� �Ȱ��� ������Ѵ�

		//finish with succes
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
