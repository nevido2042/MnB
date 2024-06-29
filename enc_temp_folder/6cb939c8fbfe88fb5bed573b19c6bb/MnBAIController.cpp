// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/MnBAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include"AICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MnBCharacter.h"

AMnBAIController::AMnBAIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void AMnBAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AAICharacter* const AICharacter = Cast<AAICharacter>(InPawn))
	{
		if (UBehaviorTree* const tree = AICharacter->GetBehaviorTree())
		{
			UBlackboardComponent* Board;
			UseBlackboard(tree->BlackboardAsset, Board);
			Blackboard = Board;
			RunBehaviorTree(tree);
		}
	}
}

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
void AMnBAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckTargetIsDie();

	RotateToTarget();
}

void AMnBAIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 1500.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees = 360.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMnBAIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense((*SightConfig));
	}
}

void AMnBAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (TargetCharacter) return; //if already Set Target return

	//same team return
	AAICharacter* ControlledPawn = Cast<AAICharacter>(GetPawn());
	AAICharacter* TempAIChar = Cast<AAICharacter>(Actor);
	if (TempAIChar == nullptr)
	{
		return;
	}

	if (ControlledPawn->GetTeam() == TempAIChar->GetTeam())
	{
		return;
	}

	TargetCharacter = TempAIChar;

	if (TargetCharacter)
	{		
		//GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
		if (GetBlackboardComponent())
		{
			GetBlackboardComponent()->SetValueAsObject("TargetCharacter", TargetCharacter);
		}
	}
}

void AMnBAIController::CheckTargetIsDie()
{
	if (IsValid(TargetCharacter) == false) return;

	if (AAICharacter* Char = Cast<AAICharacter>(TargetCharacter))
	{
		if (Char->IsDie())
		{
			TargetCharacter = nullptr;
			GetBlackboardComponent()->SetValueAsObject("TargetCharacter", TargetCharacter);
		}
	}
}

void AMnBAIController::RotateToTarget()
{

	if (TargetCharacter == nullptr) return;

	if (GetPawn() == nullptr) return; //when die can't find pawn

	const FVector MyLocation = GetPawn()->GetActorLocation();
	const FVector TargetLocation = TargetCharacter->GetActorLocation(); //���߿� �÷��̾�� Ÿ������ �ٲ㺸��

	FRotator NewControlRotation = UKismetMathLibrary::MakeRotFromX((TargetLocation - MyLocation).GetSafeNormal());
	//FRotator New = FRotator(Pawn->GetActorRotation().Pitch, NewControlRotation.Yaw, Pawn->GetActorRotation().Roll);
	FRotator NewRotation = FRotator(0.f, NewControlRotation.Yaw, NewControlRotation.Roll);

	GetPawn()->SetActorRotation(NewRotation);

	//UE_LOG(LogTemp, Display, TEXT("Pitch:%f Yaw:%f Roll:%f"), NewControlRotation.Pitch, NewControlRotation.Yaw, NewControlRotation.Roll);
	//Cont->SetControlRotation(New);

}
