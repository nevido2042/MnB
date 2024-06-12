// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Citizen.h"
#include "MnB/AI/CitizenController.h"
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
	FTransform NewTransform = FTransform
	(
		FRotator(0.0, -90.0, 0.0),
		FVector(0.0, 0.0, -90.0),
		FVector(1.0, 1.0, 1.0)
	);
	GetMesh()->SetRelativeTransform(NewTransform);


	{
		ConstructorHelpers::FObjectFinder<UBehaviorTree>Finder(TEXT("/Script/AIModule.BehaviorTree'/Game/MyAssets/AI/Citizen/BT_Citizen.BT_Citizen'"));
		if (Finder.Object)
		{
			Tree = Finder.Object;
		}
	}

	AIControllerClass = ACitizenController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	{
		ConstructorHelpers::FObjectFinder<UMaterial>Finder(TEXT("/Script/Engine.Material'/Game/MyAssets/Materials/MT_Yellow.MT_Yellow'"));
		ensure(Finder.Object);
		if (Finder.Object)
		{
			Cloth = Finder.Object;
		}
	}
}

void ACitizen::BeginPlay()
{
	AActor::BeginPlay();
	
	//if (Tree)
	//{
	//	GetBehaviorTree();
	//	// ������� �ʱ�ȭ
	//	BlackboardComp->InitializeBlackboard(*Tree->BlackboardAsset);

	//	// �ൿ Ʈ�� ����
	//	BehaviorComp->StartTree(*Tree);
	//}
}

void ACitizen::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GetMesh()->SetMaterial(7, Cloth);
}

void ACitizen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
