// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(StaticMeshComponent);

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnCapsuleBeginOverlap);
	CapsuleComponent->SetGenerateOverlapEvents(true);

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));

	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	// Enable hit events
	CapsuleComponent->SetNotifyRigidBodyCollision(true);

	// Bind hit event
	CapsuleComponent->OnComponentHit.AddDynamic(this, &AWeapon::OnHit);
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
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// Calculate the overlap position
		FVector OverlapPosition = OtherComp->GetComponentLocation();

		// Optionally, log the overlap position
		UE_LOG(LogTemp, Warning, TEXT("Overlap at position: %s"), *OverlapPosition.ToString());

		// Set the particle system location to the overlap position and activate it
		ParticleSystemComponent->SetWorldLocation(OverlapPosition);
		ParticleSystemComponent->ActivateSystem();
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

		// Set the particle system location to the hit location and activate it
		ParticleSystemComponent->SetWorldLocation(HitLocation);
		ParticleSystemComponent->ActivateSystem();
	}
}

