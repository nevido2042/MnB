// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"

#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MnB/MnBCharacter.h"
#include "Weapons/Shield.h"
#include "Kismet/GameplayStatics.h"
#include "VR/VRCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

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

#include "AI/MnBAIController.h"
void AWeapon::Equipped(AController* Controller)
{
	if (Controller)
	{
		StaticMeshComponent->SetSimulatePhysics(false);
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		Owner = Controller;
	}
}

bool AWeapon::HitDitect()
{
	FVector HitStart = StaticMeshComponent->GetSocketLocation(TEXT("HitStart"));
	FVector HitEnd = StaticMeshComponent->GetSocketLocation(TEXT("HitEnd"));

	FHitResult HitResult;
	//ĸ�� �õ� ��
	float CapsuleRadius = 50.0f;
	float CapsuleHalfHeight = 100.0f;

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Owner->GetPawn());
	
	//UKismetSystemLibrary::SphereTraceSingleByProfile(GetWorld(), HitStart, HitEnd, 5.f, TEXT("Hitable"), false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Green);
	//UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), HitStart, HitEnd, 5.f, 5.f,ETraceTypeQuery::TraceTypeQuery4, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Green);
	//GetWorld()->SweepSingleByChannel(HitResult, HitStart, HitEnd, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel4, FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight));
	//DrawDebugCapsule(GetWorld(), HitResult.Location, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, FColor::Green, false, 2.f);
	//DrawDebugCylinder(GetWorld(), HitStart, HitEnd, 5.f, 1, FColor::Green,false, 2.f);
	//DrawDebugLine(GetWorld(), HitStart, HitEnd, FColor::Green, false, 2.f);
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), HitStart, HitEnd, 5.f, TraceTypeQuery4, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Green, FLinearColor::Red, 0.5f);

	if (bHit)
	{
		if (ACharacter* Character = Cast<ACharacter>(HitResult.GetActor()))
		{
			if (Character->GetController() == Owner)
			{
				bHit = false;
				return bHit;
			}
		}

		DrawDebugLine(
			GetWorld(),
			HitStart,
			HitResult.Location,
			FColor::Red,
			false, 10.0f, 0, 1.0f
		);


		DrawDebugSphere(
			GetWorld(),
			HitResult.Location,
			2.0f,
			24,
			FColor::Red,
			false, 10.0f
		);
		

		if (Cast<AShield>(HitResult.GetActor()))
		{
			AMnBCharacter* MnBCharacter = Cast<AMnBCharacter>(Owner->GetCharacter());
			if (MnBCharacter)
			{
				Owner->GetCharacter()->StopAnimMontage();
				MnBCharacter->Blocked();
			}
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
	DrawDebugLine(GetWorld(), RayStart, RayEnd, FColor::Black, false, 0.5f);
	if (bHit)
	{

		if (ACharacter* Character = Cast<ACharacter>(HitResult.GetActor()))
		{
			if (Character->GetController() == Owner)
			{
				bHit = false;
				return bHit;
			}
		}

		DrawDebugLine(
			GetWorld(),
			RayStart,
			HitResult.Location,
			FColor::Cyan,
			false, 10.0f, 0, 1.0f
		);


		DrawDebugSphere(
			GetWorld(),
			HitResult.Location,
			2.0f,
			24,
			FColor::Blue,
			false, 10.0f
		);

		//test
		Owner->GetCharacter()->StopAnimMontage();
		AMnBCharacter* MnBCharacter = Cast<AMnBCharacter>(Owner->GetCharacter());
		if (MnBCharacter)
		{
			
			MnBCharacter->Blocked();
		}

	}
	
	return bHit;
}

void AWeapon::Unequipped()
{
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
