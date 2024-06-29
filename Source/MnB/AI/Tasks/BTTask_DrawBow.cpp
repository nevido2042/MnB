// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_DrawBow.h"
#include "BTTask_DrawBow.h"

#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AICharacter.h"
#include "AI/MnBAIController.h"
#include "AnimInstance/MnBCharacterAnimInstance.h"

UBTTask_DrawBow::UBTTask_DrawBow()
{
	NodeName = TEXT("Draw Bow");
}

EBTNodeResult::Type UBTTask_DrawBow::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	Anim->SetBowDraw(true);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}
