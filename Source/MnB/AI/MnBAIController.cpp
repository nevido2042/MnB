// Fill out your copyright notice in the Description page of Project Settings.

#include"AICharacter.h"
#include "AI/MnBAIController.h"

AMnBAIController::AMnBAIController(FObjectInitializer const& ObjectInitializer)
{
}

void AMnBAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AAICharacter* const AICharacter = Cast<AAICharacter>(InPawn))
	{
		if (UBehaviorTree* const tree = AICharacter->GetBehaviorTree())
		{
			UBlackboardComponent* Board;
			UseBlackboard(tree->BlackboardAsset, Board);
			Blackboard = Board;
			RunBehaviorTree(tree);
		}
	}
}
