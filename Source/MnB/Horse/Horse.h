// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/InteractableActor.h"
#include "Horse.generated.h"

UCLASS()
class MNB_API AHorse : public ACharacter, public IInteractableActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHorse();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Interact(AActor* InActor) override;

protected:
	class UHealth* Health = nullptr;

private:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
