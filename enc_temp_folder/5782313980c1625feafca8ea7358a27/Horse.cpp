// Fill out your copyright notice in the Description page of Project Settings.


#include "Horse/Horse.h"

#include "Components/Health.h"

// Sets default values
AHorse::AHorse()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = CreateDefaultSubobject<UHealth>(TEXT("Health"));

}

// Called when the game starts or when spawned
void AHorse::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHorse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHorse::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHorse::Interact(AActor* InActor)
{
}

float AHorse::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health->AddCurrentHP(-Damage);

	if (FMath::IsNearlyZero(Health->GetCurrentHP()))
	{
		Destroy();
	}

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

