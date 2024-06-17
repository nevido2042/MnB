// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/WarriorSpawner.h"
#include "AI/AICharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWarriorSpawner::AWarriorSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	{
		ConstructorHelpers::FClassFinder<AAICharacter>Finder(TEXT("/Script/Engine.Blueprint'/Game/MyAssets/AI/Warrior/BP_AIWarrior.BP_AIWarrior_C'"));
		if (Finder.Class)
		{
			Warrior = Finder.Class;
		}
	}

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
}

// Called when the game starts or when spawned
void AWarriorSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	//SpawnRandomPosition();
}

// Called every frame
void AWarriorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Warrior, OutActors);

	if (OutActors.Num() < 5)
	{
		SpawnRandomPosition();
	}

}

void AWarriorSpawner::SpawnRandomPosition()
{
	AActor* Actor = GetWorld()->SpawnActor(Warrior);

	if (Actor == nullptr) return;

	int RandomNum = UKismetMathLibrary::RandomInteger(SpawnPositions.Num());
	FVector WorldPosition = GetActorTransform().TransformPosition(SpawnPositions[RandomNum]);

	Actor->SetActorLocation(WorldPosition);
}

