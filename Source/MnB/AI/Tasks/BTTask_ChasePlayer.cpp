// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_ChasePlayer.h"

#include "AI/MnBAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include"Blueprint/AIBlueprintHelperLibrary.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer(FObjectInitializer const& ObjectInitializer)
	: UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = 
		"Chase Player");
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Cont = Cast<AMnBAIController>(OwnerComp.GetAIOwner()))
	{
		auto const PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

		// move to palyer location
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(Cont, PlayerLocation);

		//finish with succes
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
