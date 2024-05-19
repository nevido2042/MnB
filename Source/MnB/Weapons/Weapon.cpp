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

	//CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	//CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnCapsuleBeginOverlap);
	CapsuleComponent->SetGenerateOverlapEvents(true);

	//ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
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
	FTransform HitTransform = FTransform(SweepResult.Location);
	/*ParticleSystemComponent->SetWorldTransform(HitTransform);
	ParticleSystemComponent->ActivateSystem();*/
}

