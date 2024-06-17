// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarriorSpawner.generated.h"

UCLASS()
class MNB_API AWarriorSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarriorSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	TSubclassOf<class AAICharacter> Warrior = nullptr;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"))
	TArray<FVector> SpawnPositions;

	USceneComponent* SceneComponent = nullptr;

	void SpawnRandomPosition();

	UPROPERTY(EditAnywhere)
	bool bTick = false;

};
