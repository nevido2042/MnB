// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MnB/MnBCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
	
	StaticMeshComponent->SetSimulatePhysics(true);

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//HitDitect();
}

void AWeapon::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));
	Equipped();
}

void AWeapon::Equipped()
{
	AMnBCharacter* MnBCharacter = Cast<AMnBCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (MnBCharacter)
	{
		StaticMeshComponent->SetSimulatePhysics(false);
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		MnBCharacter->Equip();
	}
}

void AWeapon::HitDitect()
{
	FVector HitStart = StaticMeshComponent->GetSocketLocation(TEXT("HitStart"));
	FVector HitEnd = StaticMeshComponent->GetSocketLocation(TEXT("HitEnd"));

	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, HitStart, HitEnd, ECollisionChannel::ECC_GameTraceChannel4);
	DrawDebugLine(GetWorld(), HitStart, HitEnd, FColor::Green, false, 2.f);
	if (bHit)
	{

		DrawDebugLine(
			GetWorld(),
			HitStart,
			HitResult.Location,
			FColor::Red,
			false, 5.0f, 0, 1.0f
		);


		DrawDebugSphere(
			GetWorld(),
			HitResult.Location,
			2.0f,
			24,
			FColor::Red,
			false, 5.0f
		);
	}
}

void AWeapon::Unequipped()
{
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
