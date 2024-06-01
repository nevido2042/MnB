// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_FindPathPoint.h"

#include "MnB/AI/AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MnB/AI/MnBAIController.h"

UBTTask_FindPathPoint::UBTTask_FindPathPoint(FObjectInitializer const& ObjectInitializer)
	: UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = TEXT("Find Path Point");
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Cont = Cast<AMnBAIController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const Board = OwnerComp.GetBlackboardComponent())
		{
			auto const Index = Board->GetValueAsInt(GetSelectedBlackboardKey());

			if (auto* AICharacter = Cast<AAICharacter>(Cont->GetPawn()))
			{
				auto const Point = AICharacter->GetPatrolPath()->GetPatrolPoint(Index);

				auto const GlobalPoint = AICharacter->GetPatrolPath()->GetActorTransform().TransformPosition(Point);
				Board->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
