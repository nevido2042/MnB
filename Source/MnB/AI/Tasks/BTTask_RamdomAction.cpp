// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_RamdomAction.h"
#include "BTTask_RamdomAction.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AICharacter.h"
#include "AI/MnBAIController.h"
#include "AnimInstance/MnBCharacterAnimInstance.h"

UBTTask_RamdomAction::UBTTask_RamdomAction()
{
	NodeName = TEXT("Ramdom Action");
}

EBTNodeResult::Type UBTTask_RamdomAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (OutOfRange)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	SetRandomAction();
	ExecuteAction(OwnerComp);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}

void UBTTask_RamdomAction::SetRandomAction()
{
	RandomAction = (Action)(UKismetMathLibrary::RandomInteger(4));
}

void UBTTask_RamdomAction::ExecuteAction(UBehaviorTreeComponent& OwnerComp)
{
	auto const* const Cont = OwnerComp.GetAIOwner();
	auto* const AICharacter = Cast<AAICharacter>(Cont->GetPawn());
	auto* const Mesh = AICharacter->GetMesh();
	UMnBCharacterAnimInstance * Anim = Cast< UMnBCharacterAnimInstance>(Mesh->GetAnimInstance());
	switch (RandomAction)
	{
	case UBTTask_RamdomAction::Action::LeftAttack:
	{
		AICharacter->PlayAttack(true);
	}
		break;
	case UBTTask_RamdomAction::Action::RightAttack:
	{
		AICharacter->PlayAttack(false);
	}
		break;
	case UBTTask_RamdomAction::Action::LeftGuard:
	{
		Anim->SetGuardDirection(EGuardDirection::GuardLeft);
	}
		break;
	case UBTTask_RamdomAction::Action::RightGuard:
	{
		Anim->SetGuardDirection(EGuardDirection::GarudRight);
	}
		break;
	default:
		break;
	}
}
