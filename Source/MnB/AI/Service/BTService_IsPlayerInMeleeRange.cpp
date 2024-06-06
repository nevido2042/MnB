// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_IsPlayerInMeleeRange.h"

#include "MnB/AI/AICharacter.h"
#include "MnB/AI/MnBAIController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_IsPlayerInMeleeRange::UBTService_IsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UBTService_IsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const* const Cont = Cast<AMnBAIController>(OwnerComp.GetAIOwner());
	auto const* const AICharacter = Cast<AAICharacter>(Cont->GetPawn());

	auto const* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(
		GetSelectedBlackboardKey(), AICharacter->GetDistanceTo(Player) <= MeleeRange);

	//UE_LOG(LogTemp, Display, TEXT("%d"), AICharacter->GetDistanceTo(Player));
}
