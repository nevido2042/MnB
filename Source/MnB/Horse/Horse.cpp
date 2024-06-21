// Fill out your copyright notice in the Description page of Project Settings.


#include "Horse/Horse.h"

#include "Components/Health.h"
#include "Components/CapsuleComponent.h"
#include "Components/ChildActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MnBCharacter.h"

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

	//if (Rider)
	//{
	//	FVector2D MovementVector = Rider->GetMovementVector();

	//	const FRotator Rotation = Controller->GetControlRotation();
	//	const FRotator YawRotation(0, Rotation.Yaw, 0);

	//	// get forward vector
	//	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	//	// get right vector 
	//	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	//	AddMovementInput(ForwardDirection, MovementVector.Y);
	//	AddMovementInput(RightDirection, MovementVector.X);
	//}

}

// Called to bind functionality to input
void AHorse::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
#include "MnBCharacter.h"
#include "VR/VRCharacter.h"
void AHorse::Interact(AActor* InActor)
{
	if (Rider == nullptr && RiderVR == nullptr)
	{
		if (AMnBCharacter* Char = Cast<AMnBCharacter>(InActor))
		{
			Rider = Char;

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
			Char->GetCharacterMovement()->DisableMovement();

			Char->StartGetOnMontage();

			Char->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
		else if (AVRCharacter* VRChar = Cast<AVRCharacter>(InActor))
		{
			VRChar->SetCurHorse(this);
			VRChar->GetCharacterMovement()->DisableMovement();
			VRChar->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			SetCapsuleCollisionProfileName(TEXT("Vehicle"));
			VRChar->GetCharacterMovement()->GravityScale = 0.f;
			FVector SitLoc = GetSitLocation()->GetComponentLocation();
			FRotator SitRot = GetSitLocation()->GetComponentRotation();
			VRChar->SetActorLocation(SitLoc);
			VRChar->SetActorRotation(SitRot);
			VRChar->GetController()->SetControlRotation(SitRot);
			RiderVR = VRChar;
		}
	}
	else if (Rider)
	{
		Rider->StartDescendingHorseMontage();
		Rider = nullptr;
	}
	else if (RiderVR)
	{
		RiderVR->GetCharacterMovement()->GravityScale = 1.f;
		RiderVR->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		SetCapsuleCollisionProfileName(TEXT("Pawn"));

		RiderVR->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

		RiderVR->SetCurHorse(nullptr);

		RiderVR = nullptr;
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

