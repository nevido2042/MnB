// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_IncreamentPathIndex.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AICharacter.h"
#include "AI/MnBAIController.h"

UBTTask_IncreamentPathIndex::UBTTask_IncreamentPathIndex(FObjectInitializer const& ObjectInitializer)
	: UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = 
		"Increment Path Index");
}

EBTNodeResult::Type UBTTask_IncreamentPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Cont = Cast<AMnBAIController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const AICharacter = Cast<AAICharacter>(Cont->GetPawn()))
		{
			if (auto* const BC = OwnerComp.GetBlackboardComponent())
			{
				int const NoOfPoints = AICharacter->GetPatrolPath()->Num();
				auto const MinIndex = 0;
				auto const MaxIndex = NoOfPoints - 1;
				auto Index = BC->GetValueAsInt(GetSelectedBlackboardKey());

				//change direction if we are at the first or last index if we are in bidirection mode
				if (bBiDirectional)
				{
					if (Index >= MaxIndex && Direction == EDirectionType::Forward)
					{
						Direction = EDirectionType::Reverse;
					}
					else if (Index == MinIndex && Direction == EDirectionType::Reverse)
					{
						Direction = EDirectionType::Forward;
					}
				}

				BC->SetValueAsInt(GetSelectedBlackboardKey(),
					(Direction == EDirectionType::Forward ? ++Index : --Index) % NoOfPoints);

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
