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

	//{
	//	ConstructorHelpers::FClassFinder<AAICharacter>Finder(TEXT("/Script/Engine.Blueprint'/Game/MyAssets/AI/Warrior/BP_AIWarrior.BP_AIWarrior_C'"));
	//	if (Finder.Class)
	//	{
	//		Warrior = Finder.Class;
	//	}
	//}

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

	//TArray<AActor*> OutActors;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAICharacter::StaticClass(), OutActors);

	//if (OutActors.Num() < 5)
	//{
	//	SpawnRandomPosition();
	//}
	GetWorldTimerManager().SetTimer(Timer, this, &AWarriorSpawner::SpawnRandomPosition, 5.f, true);
}

void AWarriorSpawner::SpawnRandomPosition()
{
	AActor* Actor = GetWorld()->SpawnActor(RandomWarrior());

	if (Actor == nullptr) return;
	if (SpawnPositions.Num() == 0) return;

	int RandomNum = UKismetMathLibrary::RandomInteger(SpawnPositions.Num());
	FVector WorldPosition = GetActorTransform().TransformPosition(SpawnPositions[RandomNum]);

	Actor->SetActorLocation(WorldPosition);
}

TSubclassOf<class AAICharacter> AWarriorSpawner::RandomWarrior()
{
	if (Warriors.Num() == 0) return nullptr;

	int Rand = UKismetMathLibrary::RandomInteger(Warriors.Num());

	return Warriors[Rand];
}

