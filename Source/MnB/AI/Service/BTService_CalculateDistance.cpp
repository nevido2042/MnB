// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_CalculateDistance.h"

#include "MnB/AI/AICharacter.h"
#include "MnB/AI/MnBAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UBTService_CalculateDistance::UBTService_CalculateDistance()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Calculate Target Distacne");
}

void UBTService_CalculateDistance::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const* const Cont = Cast<AMnBAIController>(OwnerComp.GetAIOwner());
	auto const* const AICharacter = Cast<AAICharacter>(Cont->GetPawn());

	auto const* const Target = Cont->GetTarget();

	//auto const* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(
		GetSelectedBlackboardKey(), AICharacter->GetDistanceTo(Target) <= WantDistacne);
}
