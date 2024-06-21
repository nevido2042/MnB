// Fill out your copyright notice in the Description page of Project Settings.


#include "Horse/Horse.h"

#include "Components/Health.h"
#include "Components/CapsuleComponent.h"
#include "Components/ChildActorComponent.h"

// Sets default values
AHorse::AHorse()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = CreateDefaultSubobject<UHealth>(TEXT("Health"));

	BodyCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCapsule"));
	BodyCapsule->SetupAttachment(RootComponent);

	Left = CreateDefaultSubobject<USceneComponent>(TEXT("Left"));
	Left->SetupAttachment(GetMesh());
	Right = CreateDefaultSubobject<USceneComponent>(TEXT("Right"));
	Right->SetupAttachment(GetMesh());

	SitLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SitLocation"));
	SitLocation->SetupAttachment(GetMesh());

	/*SitLoc = CreateDefaultSubobject<UChildActorComponent>(TEXT("Person"));
	SitLoc->SetupAttachment(GetMesh());*/
}

// Called when the game starts or when spawned
void AHorse::BeginPlay()
{
	Super::BeginPlay();

	SetInfo(FText::FromString("Horse\nGet On"));
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
#include "MnBCharacter.h"
void AHorse::Interact(AActor* InActor)
{
	if (AMnBCharacter* Char = Cast<AMnBCharacter>(InActor))
	{
		float LeftDist = FVector::Dist(Left->GetComponentLocation(), Char->GetActorLocation());
		float RightDist = FVector::Dist(Right->GetComponentLocation(), Char->GetActorLocation()); 

		if (LeftDist < RightDist)
		{
			Char->SetActorLocation(Left->GetComponentLocation());
		}
		else
		{
			Char->SetActorLocation(Right->GetComponentLocation());
		}
		Char->SetCurHorse(this);

		Char->StartGetOnMontage();
	}
}

void AHorse::SetCapsuleCollisionProfileName(FName CollisionProfileName)
{
	GetCapsuleComponent()->SetCollisionProfileName(CollisionProfileName);
	BodyCapsule->SetCollisionProfileName(CollisionProfileName);
}

float AHorse::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health->AddCurrentHP(-Damage);

	/*if (FMath::IsNearlyZero(Health->GetCurrentHP()))
	{
		Destroy();
	}*/

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

