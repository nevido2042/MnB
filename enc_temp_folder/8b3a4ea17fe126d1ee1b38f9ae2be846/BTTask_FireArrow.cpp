// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_FireArrow.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AICharacter.h"
#include "AI/MnBAIController.h"
#include "AnimInstance/MnBCharacterAnimInstance.h"

UBTTask_FireArrow::UBTTask_FireArrow()
{
	NodeName = TEXT("Fire Arrow");
}

EBTNodeResult::Type UBTTask_FireArrow::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (OutOfRange)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	auto const* const Cont = OwnerComp.GetAIOwner();
	auto* const AICharacter = Cast<AAICharacter>(Cont->GetPawn());
	auto* const Mesh = AICharacter->GetMesh();
	UMnBCharacterAnimInstance* Anim = Cast< UMnBCharacterAnimInstance>(Mesh->GetAnimInstance());

	Anim->SetBowDraw(false);
	AICharacter->PlayBowRecoil();

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}
