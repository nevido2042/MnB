// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Citizen.h"
#include "MnB/AI/MnBAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

ACitizen::ACitizen()
{
	{
		ConstructorHelpers::FObjectFinder<USkeletalMesh>Finder(TEXT("/Script/Engine.SkeletalMesh'/Game/KoreanTraditionalMartialArts/Meshs/Characters/Meshs/SKM_Soldier_4.SKM_Soldier_4'"));
		if (Finder.Object)
		{
			Mesh = Finder.Object;
		}
	}
	{
		ConstructorHelpers::FClassFinder<UAnimInstance>Finder(TEXT("/ Script / Engine.AnimBlueprint'/Game/MyAssets/Character/ABP_Korea.ABP_Korea_C'"));
		if (Finder.Class)
		{
			AnimClass = Finder.Class;
		}
	}

	GetMesh()->SetSkeletalMesh(Mesh);
	GetMesh()->AddRelativeLocation(FVector(0, 0, -90.f));
	GetMesh()->SetAnimClass(AnimClass);


	{
		ConstructorHelpers::FObjectFinder<UBehaviorTree>Finder(TEXT("/Script/AIModule.BehaviorTree'/Game/MyAssets/AI/Citizen/BT_Citizen.BT_Citizen'"));
		if (Finder.Object)
		{
			Tree = Finder.Object;
		}
	}

	AIControllerClass = AMnBAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed = 200.f;
}

void ACitizen::BeginPlay()
{
	AActor::BeginPlay();
	
	//if (Tree)
	//{
	//	GetBehaviorTree();
	//	// 블랙보드 초기화
	//	BlackboardComp->InitializeBlackboard(*Tree->BlackboardAsset);

	//	// 행동 트리 시작
	//	BehaviorComp->StartTree(*Tree);
	//}
}

void ACitizen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
