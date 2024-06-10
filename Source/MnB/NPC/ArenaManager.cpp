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

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SkeletalMeshComponent->SetSkeletalMesh(Mesh);
	SkeletalMeshComponent->SetMaterial(0, ClothMaterial);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComponent->SetupAttachment(SkeletalMeshComponent);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->AddRelativeLocation(FVector(0, 0, 100.f));
	CapsuleComponent->SetCapsuleHalfHeight(80.f);
	CapsuleComponent->SetCollisionProfileName("Interactable");
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

void AArenaManager::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Arena"));
	EnterArena();
}

void AArenaManager::EnterArena()
{

}

