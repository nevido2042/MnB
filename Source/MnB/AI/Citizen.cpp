// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Citizen.h"
#include "MnB/AI/CitizenController.h"
#include "GameFramework/CharacterMovementComponent.h"

ACitizen::ACitizen()
{
	static ConstructorHelpers::FClassFinder<UAnimInstance>AnimClassAsset(TEXT("/ Script / Engine.AnimBlueprint'/Game/MyAssets/Character/ABP_Korea.ABP_Korea_C'"));
	check(AnimClassAsset.Class);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>Finder(TEXT("/Script/Engine.SkeletalMesh'/Game/KoreanTraditionalMartialArts/Meshs/Characters/Meshs/SKM_Soldier_4.SKM_Soldier_4'"));
	check(Finder.Object);

	USkeletalMeshComponent* MeshComponent = GetMesh();
	MeshComponent->SetSkeletalMesh(Finder.Object);
	MeshComponent->AddRelativeLocation(FVector(0, 0, -90.f));
	MeshComponent->SetAnimClass(AnimClassAsset.Class);
	FTransform NewTransform = FTransform
	(
		FRotator(0.0, -90.0, 0.0),
		FVector(0.0, 0.0, -90.0),
		FVector(1.0, 1.0, 1.0)
	);
	MeshComponent->SetRelativeTransform(NewTransform);

	{
		static ConstructorHelpers::FObjectFinder<UBehaviorTree>FinderAsset(TEXT("/Script/AIModule.BehaviorTree'/Game/MyAssets/AI/Citizen/BT_Citizen.BT_Citizen'"));
		if (FinderAsset.Object)
		{
			Tree = FinderAsset.Object;
		}
	}

	AIControllerClass = ACitizenController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	{
		static ConstructorHelpers::FObjectFinder<UMaterial>FinderAsset(TEXT("/Script/Engine.Material'/Game/MyAssets/Materials/MT_Yellow.MT_Yellow'"));
		ensure(FinderAsset.Object);
		if (FinderAsset.Object)
		{
			//Cloth = FinderAsset.Object;

			GetMesh()->SetMaterial(7, FinderAsset.Object);
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
}

void ACitizen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
