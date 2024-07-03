// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/WarriorAI.h"
#include "Kismet/GameplayStatics.h"
#include "Spawner/WarriorSpawner.h"

void AWarriorAI::Die()
{
	Super::Die();

	AWarriorSpawner* Spawner = Cast<AWarriorSpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AWarriorSpawner::StaticClass()));
	if (Spawner)
	{
		Spawner->SpawnRandomPosition();
	}
}
