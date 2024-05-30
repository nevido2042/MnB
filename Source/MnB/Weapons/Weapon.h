// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MnB/Interface/InteractableActor.h"
#include "Weapon.generated.h"

UCLASS()
class MNB_API AWeapon : public AActor, public IInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

protected:

	virtual void Interact() override;

	void Equipped();

public:
	void Unequipped();

	void HitDitect();
};
