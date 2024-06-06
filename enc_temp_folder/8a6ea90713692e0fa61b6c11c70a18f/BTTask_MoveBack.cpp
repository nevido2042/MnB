// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_MoveBack.h"

#include "AI/MnBAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"

UBTTask_MoveBack::UBTTask_MoveBack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Move Back");
}

EBTNodeResult::Type UBTTask_MoveBack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Cont = Cast<AMnBAIController>(OwnerComp.GetAIOwner()))
	{
		auto const bTooClose = OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());

		auto const* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		const FVector MyLocation = Cont->GetPawn()->GetActorLocation();
		const FVector TargetLocation = Player->GetActorLocation(); //나중에 플레이어말고 타겟으로 바꿔보자

		FVector Direction = (MyLocation - TargetLocation).GetSafeNormal();

		float MoveAmount = 100.f;

		FVector FleeLocation = MyLocation + Direction * (MoveAmount);

		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
		FNavLocation NavLocation;

		if (NavSys->GetRandomPointInNavigableRadius(FleeLocation, 200.0f, NavLocation))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(Cont, NavLocation);
		}

		//finish with succes
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
