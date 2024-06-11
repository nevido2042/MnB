// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"

#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MnB/MnBCharacter.h"
#include "Weapons/Shield.h"
#include "Kismet/GameplayStatics.h"
#include "VR/VRCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"

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

	GuardCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("GuardCollider"));
	GuardCollider->SetupAttachment(StaticMeshComponent);
	GuardCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GuardCollider->SetCollisionProfileName(TEXT("Gaurd"));

	{
		ConstructorHelpers::FObjectFinder<UParticleSystem>Finder(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Blood/P_Blood_Splat_Cone.P_Blood_Splat_Cone'"));
		ensure(Finder.Object);
		if (Finder.Object)
		{
			Particles.Add(Finder.Object);
		}
	}
	{
		ConstructorHelpers::FObjectFinder<UParticleSystem>Finder(TEXT("	/ Script / Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Hit/P_Default.P_Default'"));
		ensure(Finder.Object);
		if (Finder.Object)
		{
			Particles.Add(Finder.Object);
		}
	}
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleComponent->SetTemplate(Particles[0]); //blood 0
	ParticleComponent->SetAutoActivate(false);
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

	if (Owner == nullptr) return;

	if (Cast<AVRCharacter>(Owner->GetPawn()))
	{
		HitDitect();
	}
}

void AWeapon::Interact(AActor* InActor)
{
	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));
	Equipped();
}

void AWeapon::SetWeaponHitable()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, TEXT("SetWeaponHitable"));
	bApplyDamage = false;
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



	if (Owner == nullptr) return;

	if (Cast<AVRCharacter>(Owner->GetPawn()))
	{
		GetWorldTimerManager().SetTimer(Timer, this, &AWeapon::SetWeaponHitable, 1.f, true); //왜 반복 안하는가`

		GuardCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AWeapon::HitDitect()
{
	if (bApplyDamage) return; //데미지 한번 만 줄수 있게

	FVector HitStart = StaticMeshComponent->GetSocketLocation(TEXT("HitStart"));
	FVector HitEnd = StaticMeshComponent->GetSocketLocation(TEXT("HitEnd"));

	FHitResult HitResult;
	//ĸ�� �õ� ��
	float CapsuleRadius = 50.0f;
	float CapsuleHalfHeight = 100.0f;

	TArray<AActor*> IgnoreActors;

	if (Owner)
	{
		IgnoreActors.Add(Owner->GetPawn()); //when die can't find pawn
	}
	IgnoreActors.Add(this);
	
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
				return;
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

		if (Cast<AWeapon>(HitResult.GetActor()))
		{
			if (Owner)
			{
				Owner->GetCharacter()->StopAnimMontage();

				ParticleComponent->SetTemplate(Particles[1]);
				ParticleComponent->SetWorldLocation(HitResult.ImpactPoint);
				ParticleComponent->ActivateSystem();

				return;
			}
			//MnBCharacter->Blocked();	
		}
		

		if (Cast<AShield>(HitResult.GetActor()))
		{
			AMnBCharacter* MnBCharacter = Cast<AMnBCharacter>(Owner->GetCharacter());
			if (MnBCharacter)
			{
				if (Owner)
				{
					Owner->GetCharacter()->StopAnimMontage();
					MnBCharacter->Blocked();

					ParticleComponent->SetTemplate(Particles[1]);
					ParticleComponent->SetWorldLocation(HitResult.ImpactPoint);
					ParticleComponent->ActivateSystem();

					return;
				}

			}
		}

		ParticleComponent->SetTemplate(Particles[0]);
		ParticleComponent->SetWorldLocation(HitResult.ImpactPoint);
		ParticleComponent->ActivateSystem();

		UGameplayStatics::ApplyDamage(HitResult.GetActor(), 1, Owner, this, nullptr);

		bApplyDamage = true;

	}

	if (bApplyDamage == false)
	{
		ObstacleDitect();
	}
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
		if (Owner)
		{
			Owner->GetCharacter()->StopAnimMontage();

			AMnBCharacter* MnBCharacter = Cast<AMnBCharacter>(Owner->GetCharacter());
			if (MnBCharacter)
			{

				MnBCharacter->Blocked();
			}
		}

		ParticleComponent->SetTemplate(Particles[1]);
		ParticleComponent->SetWorldLocation(HitResult.ImpactPoint);
		ParticleComponent->ActivateSystem();

	}
	
	return bHit;
}

void AWeapon::Unequipped()
{
	Owner = nullptr;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	StaticMeshComponent->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
	StaticMeshComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetWorldTimerManager().ClearTimer(Timer);
}
