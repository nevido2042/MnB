// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/MnBAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include"AICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MnBCharacter.h"
#include "Horse/Horse.h"
#include "AI/WarriorAI.h"
#include "Character/WorldCharacter.h"
AMnBAIController::AMnBAIController(FObjectInitializer const& ObjectInitializer)
{
	//SetupPerceptionSystem();
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

	GetBlackboardComponent()->SetValueAsBool("IsCharge", true);

}

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
void AMnBAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//CheckTargetIsDie();
	
	//SearchTarget();

	RotateToTarget();
}
#include "Kismet/KismetSystemLibrary.h"
void AMnBAIController::SetTarget(AActor* NewTarget)
{
	TargetCharacter = NewTarget;
	GetBlackboardComponent()->SetValueAsObject("TargetCharacter", TargetCharacter);
}
void AMnBAIController::ClearTarget()
{
	TargetCharacter = nullptr;
	GetBlackboardComponent()->SetValueAsObject("TargetCharacter", TargetCharacter);
}
void AMnBAIController::SearchTarget()
{
	if (TargetCharacter && GetPawn() && GetWorld())
	{
		return;
	}
	if (AAICharacter* AIChar = Cast<AAICharacter>(GetPawn()))
	{
		if (AIChar->IsDie())
		{
			return;
		}
	}

	TArray<TEnumAsByte<EObjectTypeQuery>> Array;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetPawn());
	TArray<class AActor*> OutActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetPawn()->GetActorLocation(), SearchRadius, Array, ACharacter::StaticClass(), ActorsToIgnore, OutActors);

	float MinDist = -1.f;
	AActor* MinDistActor = nullptr;
	AAICharacter* ControlledPawn = Cast<AAICharacter>(GetPawn());
	for (AActor* Actor : OutActors)
	{
		//check warrior and player
		if (!Cast<AWarriorAI>(Actor)&&!Cast<AMnBCharacter>(Actor)&&!Cast<AWorldCharacter>(Actor))
		{
			continue;
		}

		//horse check
		if (Cast<AHorse>(Actor))
		{
			continue;
		}

		//team check
		AMnBCharacter* TempPlayerChar = Cast<AMnBCharacter>(Actor);
		if (TempPlayerChar)
		{
			if (ControlledPawn->GetTeam() != ETeam::NoTeam)
			{
				if (ControlledPawn->GetTeam() == TempPlayerChar->GetTeam())
				{
					continue;
				}
			}
		}
		AAICharacter* TempAIChar = Cast<AAICharacter>(Actor);
		if (TempAIChar)
		{
			if (ControlledPawn->GetTeam() != ETeam::NoTeam)
			{
				if (ControlledPawn->GetTeam() == TempAIChar->GetTeam())
				{
					continue;
				}
			}
		}
		
		//distance check
		float NewDist = FVector::Dist(GetPawn()->GetActorLocation(), Actor->GetActorLocation());

		if (MinDist == -1.f)
		{
			MinDist = NewDist;
			MinDistActor = Actor;
		}
		else if (MinDist > NewDist)
		{
			MinDist = NewDist;
			MinDistActor = Actor;
		}
	}

	if (MinDistActor == nullptr)
	{
		return;
	}

	TargetCharacter = MinDistActor;

	//TargetCharacter = TempAIChar;

	if (TargetCharacter)
	{
		//GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
		if (GetBlackboardComponent())
		{
			GetBlackboardComponent()->SetValueAsObject("TargetCharacter", TargetCharacter);
		}
	}
}

void AMnBAIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 99999.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees = 360.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMnBAIController::OnTargetDetected);
		GetPerceptionComponent()->OnTargetPerceptionForgotten.AddDynamic(this, &AMnBAIController::OnTargetForgottened);
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

void AMnBAIController::OnTargetForgottened(AActor* Actor)
{
	TargetCharacter = nullptr;
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
			/*GetBlackboardComponent()->SetValueAsBool("IsDie", false);*/
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
