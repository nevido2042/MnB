// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacter.h"
#include "Citizen.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API ACitizen : public AAICharacter
{
	GENERATED_BODY()

public:
	ACitizen();

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	USkeletalMesh* Mesh = nullptr;

	TSubclassOf<UAnimInstance> AnimClass = nullptr;
	
	UMaterial* Cloth = nullptr;
};
