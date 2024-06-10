// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableActor.h"
#include "ArenaManager.generated.h"

UCLASS()
class MNB_API AArenaManager : public AActor, public IInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArenaManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	USkeletalMeshComponent* SkeletalMeshComponent = nullptr;
	USkeletalMesh* Mesh = nullptr;

	UMaterial* ClothMaterial = nullptr;

	class UCapsuleComponent* CapsuleComponent = nullptr;

	virtual void Interact() override;

	void EnterArena();

};
