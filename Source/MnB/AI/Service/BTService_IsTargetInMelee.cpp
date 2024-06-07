// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_IsTargetInMelee.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "MnB/AI/MnBAIController.h"
#include "MnB/AI/AICharacter.h"

UBTService_IsTargetInMelee::UBTService_IsTargetInMelee()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Target In Melee Range");
}

void UBTService_IsTargetInMelee::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const* const Cont = Cast<AMnBAIController>(OwnerComp.GetAIOwner());
	//auto const* const AICharacter = Cast<AAICharacter>(Cont->GetPawn());

	auto const* const Target = Cont->GetTarget();

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(
		GetSelectedBlackboardKey(), Cont->GetPawn()->GetDistanceTo(Target) <= MeleeRange);

}
