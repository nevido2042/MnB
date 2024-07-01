// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Arrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(StaticMeshComponent);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (Cast<ACharacter>(OtherActor) == nullptr)
	//{
	//	return;
	//}

	ProjectileMovementComponent->StopMovementImmediately();
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	UGameplayStatics::ApplyDamage(OtherActor, 1.f, nullptr, nullptr, nullptr);

	if (OtherActor->GetComponentByClass<USkeletalMeshComponent>())
	{
		AttachToComponent(OtherActor->GetComponentByClass<USkeletalMeshComponent>(), FAttachmentTransformRules::KeepWorldTransform);
	}

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

