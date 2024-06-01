// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_FindRandomLocation.h"

#include "NavigationSystem.h"
#include "MnB/AI/MnBAIController.h"
//#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
	: UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = "Find Random Location In NavMesh";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AMnBAIController* const Controller = Cast<AMnBAIController>(OwnerComp.GetAIOwner()))
	{
		if (APawn* const Pawn = Controller->GetPawn())
		{
			FVector const OriginVector = Pawn->GetActorLocation();

			if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation Loc;
				if (NavSys->GetRandomPointInNavigableRadius(OriginVector, SearchRadius, Loc))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
				}

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
