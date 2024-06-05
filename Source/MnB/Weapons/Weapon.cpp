// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"

#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MnB/MnBCharacter.h"
#include "Weapons/Shield.h"
#include "Kismet/GameplayStatics.h"
#include "VR/VRCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	WeaponGrip = CreateDefaultSubobject< USceneComponent>(TEXT("WeaponGrip"));
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("GrabDirection"));

	SetRootComponent(SceneComponent);
	
	StaticMeshComponent->SetupAttachment(RootComponent);

	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCanEverAffectNavigation(false);

	WeaponGrip->SetupAttachment(RootComponent);
	Arrow->SetupAttachment(WeaponGrip);

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

		Owner = MnBCharacter->GetController();
	}

	AVRCharacter* VRCharacter = Cast<AVRCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (VRCharacter)
	{
		StaticMeshComponent->SetSimulatePhysics(false);
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		Owner = VRCharacter->GetController();
	}
}

bool AWeapon::HitDitect()
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
		
		if (Cast<AShield>(HitResult.GetActor()))
		{
			Owner->GetCharacter()->StopAnimMontage();

			//test
			Cast<AMnBCharacter>(Owner->GetCharacter())->Blocked();
		}

		UGameplayStatics::ApplyDamage(HitResult.GetActor(), 1, Owner, this, nullptr);

	}

	return bHit;
}

bool AWeapon::ObstacleDitect()
{
	FVector RayStart = StaticMeshComponent->GetSocketLocation(TEXT("RayStart"));
	FVector RayEnd = StaticMeshComponent->GetSocketLocation(TEXT("RayEnd"));

	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, RayStart, RayEnd, ECollisionChannel::ECC_Visibility);
	DrawDebugLine(GetWorld(), RayStart, RayEnd, FColor::Black, false, 2.f);
	if (bHit)
	{

		DrawDebugLine(
			GetWorld(),
			RayStart,
			HitResult.Location,
			FColor::Cyan,
			false, 5.0f, 0, 1.0f
		);


		DrawDebugSphere(
			GetWorld(),
			HitResult.Location,
			2.0f,
			24,
			FColor::Blue,
			false, 5.0f
		);

		Owner->GetCharacter()->StopAnimMontage();

		//test
		Cast<AMnBCharacter>(Owner->GetCharacter())->Blocked();

	}
	
	return bHit;
}

void AWeapon::Unequipped()
{
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
