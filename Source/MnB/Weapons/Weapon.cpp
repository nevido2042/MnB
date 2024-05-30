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

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(StaticMeshComponent);

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnCapsuleBeginOverlap);
	CapsuleComponent->SetGenerateOverlapEvents(true);

	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Enable hit events
	CapsuleComponent->SetNotifyRigidBodyCollision(true);

	// Bind hit event
	CapsuleComponent->OnComponentHit.AddDynamic(this, &AWeapon::OnHit);
	
	//CapsuleComponent->SweepComponent()
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

}

void AWeapon::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bFromSweep)
	{
		// Get the bone name if the hit result has a valid hit bone name
		if (SweepResult.BoneName.IsValid())
		{
			FName BoneName = SweepResult.BoneName;
			FString BoneNameString = BoneName.ToString();

			// Do something with the bone name
			// For example, print it to the log
			UE_LOG(LogTemp, Warning, TEXT("Collision with bone: %s"), *BoneNameString);
		}
		else
		{
			// If the hit result doesn't have a valid hit bone name
			UE_LOG(LogTemp, Warning, TEXT("Collision with unknown bone"));
		}
	}


	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// Calculate the overlap position
		FVector OverlapPosition = OtherComp->GetComponentLocation();
		

		// Optionally, log the overlap position
		UE_LOG(LogTemp, Warning, TEXT("Overlap at position: %s, BoneName: %s"), *OverlapPosition.ToString(), SweepResult.BoneName);

		// Set the particle system location to the overlap position and activate it
	}

	/*FTransform HitTransform = FTransform(SweepResult.Location);
	ParticleSystemComponent->SetWorldTransform(HitTransform);
	ParticleSystemComponent->ActivateSystem();*/
}

void AWeapon::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// Get the hit location
		FVector HitLocation = Hit.ImpactPoint;

		// Optionally, log the hit location
		UE_LOG(LogTemp, Warning, TEXT("Hit at location: %s"), *HitLocation.ToString());
	}
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

		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		MnBCharacter->Equip();
	}
}

void AWeapon::Unequipped()
{
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
