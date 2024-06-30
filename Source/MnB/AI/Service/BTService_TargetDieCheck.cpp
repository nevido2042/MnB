// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_TargetDieCheck.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "MnB/AI/MnBAIController.h"
#include "MnB/AI/AICharacter.h"

UBTService_TargetDieCheck::UBTService_TargetDieCheck()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Target Die?");
}

void UBTService_TargetDieCheck::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICharacter* AICharacter = Cast<AAICharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetCharacter")));
	if (AICharacter)
	{
		if (AICharacter->IsDie())
		{
			Cast<AMnBAIController>(OwnerComp.GetAIOwner())->ClearTarget();
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("TargetCharacter"), nullptr);
		}
	}
}
