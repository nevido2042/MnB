// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_CalculateDistance.h"

#include "MnB/AI/AICharacter.h"
#include "MnB/AI/MnBAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UBTService_CalculateDistance::UBTService_CalculateDistance()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Calculate Target Distacne");
}

void UBTService_CalculateDistance::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const* const Cont = Cast<AMnBAIController>(OwnerComp.GetAIOwner());
	auto const* const AICharacter = Cast<AAICharacter>(Cont->GetPawn());

	auto const* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(
		GetSelectedBlackboardKey(), AICharacter->GetDistanceTo(Player) <= WantDistacne);

	
	//UE_LOG(LogTemp, Display, TEXT("%d"), AICharacter->GetDistanceTo(Player));

	const FVector MyLocation = Cont->GetPawn()->GetActorLocation();
	const FVector TargetLocation = Player->GetActorLocation(); //나중에 플레이어말고 타겟으로 바꿔보자

	FRotator NewControlRotation = UKismetMathLibrary::MakeRotFromX((TargetLocation - MyLocation).GetSafeNormal());
	Cont->GetPawn()->SetActorRotation(NewControlRotation);
}
