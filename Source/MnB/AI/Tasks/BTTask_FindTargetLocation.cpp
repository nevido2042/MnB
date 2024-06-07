// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_FindTargetLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"

UBTTask_FindTargetLocation::UBTTask_FindTargetLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Target Location");
}

EBTNodeResult::Type UBTTask_FindTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UObject* Obj = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetCharacter"));

	if (ACharacter* TargetCharacter = Cast<ACharacter>(Obj))
	{
		auto const TargetLocation = TargetCharacter->GetActorLocation();
		if (SearchRandom)
		{
			FNavLocation Loc;
			if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				if (NavSys->GetRandomPointInNavigableRadius(TargetLocation, SearchRadius, Loc))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), TargetLocation);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
