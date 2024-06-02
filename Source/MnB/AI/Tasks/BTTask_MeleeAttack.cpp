// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_MeleeAttack.h"

#include "MnB/AI/CombatInterface.h"
#include "AI/MnBAIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (OutOfRange)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	auto const* const Cont = OwnerComp.GetAIOwner();
	auto* const AICharacter = Cast<AAICharacter>(Cont->GetPawn());

	if (auto* const ICombat = Cast<ICombatInterface>(AICharacter))
	{
		if (MontageHasFinished(AICharacter))
		{
			ICombat->Execute_MeleeAttack(AICharacter);
		}
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}

bool UBTTask_MeleeAttack::MontageHasFinished(AAICharacter* const AICharacter)
{
	return AICharacter->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(AICharacter->GetMontage());
}
