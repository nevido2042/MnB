// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/ArenaManager.h"

#include "Components/CapsuleComponent.h"

// Sets default values
AArenaManager::AArenaManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	{
		ConstructorHelpers::FObjectFinder<USkeletalMesh>Finder(TEXT("/Script/Engine.SkeletalMesh'/Game/KoreanTraditionalMartialArts/Meshs/Characters/Meshs/SKM_Soldier_3.SKM_Soldier_3'"));
		if (Finder.Object)
		{
			Mesh = Finder.Object;
		}
	}
	{
		ConstructorHelpers::FObjectFinder<UMaterial>Finder(TEXT("/Script/Engine.Material'/Game/MyAssets/Materials/MT_Brown.MT_Brown'"));
		if (Finder.Object)
		{
			ClothMaterial = Finder.Object;
		}
	}
	{
		ConstructorHelpers::FClassFinder<UAnimInstance>Finder(TEXT("/ Script / Engine.AnimBlueprint'/Game/MyAssets/Character/ABP_Korea.ABP_Korea_C'"));
		if (Finder.Class)
		{
			AnimClass = Finder.Class;
		}
	}

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->SetCapsuleHalfHeight(80.f);
	CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	CapsuleComponent->SetCollisionProfileName("Pawn");

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SkeletalMeshComponent->SetSkeletalMesh(Mesh);
	SkeletalMeshComponent->SetMaterial(0, ClothMaterial);
	SkeletalMeshComponent->SetCollisionProfileName("Interactable");
	SkeletalMeshComponent->AddRelativeLocation(FVector(0, 0, -100.f));
	SkeletalMeshComponent->SetAnimClass(AnimClass);
}

// Called when the game starts or when spawned
void AArenaManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArenaManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArenaManager::Interact(AActor* InActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Arena"));
	EnterArena(InActor);
	CachedActor = InActor;
}

void AArenaManager::EnterArena(AActor* InActor)
{
	FVector const GlobalPoint = GetActorTransform().TransformPosition(EnterPosition);
	InActor->SetActorLocation(GlobalPoint);

}

#include "Components/Health.h"

void AArenaManager::RecoveryAndReturn()
{
	if (CachedActor)
	{
		FVector const GlobalPoint = GetActorTransform().TransformPosition(ReturnPosion);
		CachedActor->SetActorLocation(GlobalPoint);

		UHealth* Health = CachedActor->GetComponentByClass<UHealth>();
		Health->SetCurrentHP(Health->GetMaxHP());
	}
}

