// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MnB/Interface/InteractableActor.h"
#include "Components/ArrowComponent.h"
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
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent = nullptr;

	UPROPERTY(EditAnywhere)
	USceneComponent* WeaponGrip = nullptr;

	UArrowComponent* Arrow = nullptr;

	AController* Owner;

protected:

	virtual void Interact() override;

	void Equipped();

public:
	USceneComponent* GetWeaponGrip() { return WeaponGrip; }

	void Unequipped();

	bool HitDitect();

	bool ObstacleDitect();
};
