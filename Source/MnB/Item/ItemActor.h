// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableActor.h"
#include "ItemActor.generated.h"

UCLASS()
class MNB_API AItemActor : public AActor, public IInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* InActor = nullptr) override;

protected:
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle DataHandle;

	struct FItemData* DataRow = nullptr;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* Collider = nullptr;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh = nullptr;
};
