// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"

#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MnB/MnBCharacter.h"
#include "Weapons/Shield.h"
#include "Kismet/GameplayStatics.h"
#include "VR/VRCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"

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
		static ConstructorHelpers::FObjectFinder<UParticleSystem>Finder(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Blood/P_Blood_Splat_Cone.P_Blood_Splat_Cone'"));
		ensure(Finder.Object);
		if (Finder.Object)
		{
			Particles.Add(Finder.Object);
		}
	}
	{
		static ConstructorHelpers::FObjectFinder<UParticleSystem>Finder(TEXT("	/ Script / Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Hit/P_Default.P_Default'"));
		ensure(Finder.Object);
		if (Finder.Object)
		{
			Particles.Add(Finder.Object);
		}
	}
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleComponent->SetTemplate(Particles[0]); //blood 0
	ParticleComponent->SetAutoActivate(false);

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	Audio->SetupAttachment(StaticMeshComponent);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	//SetInfo(FText::FromString("WeaponName\nPick Up Weapon"));
	SetInfo(FText::Format(FTextFormat::FromString("{0}\nPick Up"), WeaponName));

}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Owner == nullptr) return;

	if (bOwnerVR)
	{
		HitDetect();
	}
}

void AWeapon::Interact(AActor* InActor)
{
	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));
	Equipped();
}

void AWeapon::SetWeaponHitable()
{
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, TEXT("SetWeaponHitable"));
	bApplyDamage = false;
	bHitVRMode = false;
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
		bOwnerVR = true;

		GetWorldTimerManager().SetTimer(Timer, this, &AWeapon::SetWeaponHitable, 1.f, true); //왜 반복 안하는가`

		GuardCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AWeapon::HitDetectStart()
{
	LastHitStart = StaticMeshComponent->GetSocketLocation(TEXT("HitStart"));
	LastHitEnd = StaticMeshComponent->GetSocketLocation(TEXT("HitEnd"));
}

void AWeapon::HitDetect()
{
	if (bApplyDamage) return; //데미지 한번 만 줄수 있게

	if (bOwnerVR && bHitVRMode)
	{
		return;
	}

	FVector HitStart = StaticMeshComponent->GetSocketLocation(TEXT("HitStart"));
	FVector HitEnd = StaticMeshComponent->GetSocketLocation(TEXT("HitEnd"));

	struct FLastCache
	{
		FLastCache(AWeapon* InWeapon, 
			const FVector& InStart, const FVector& InEnd) 
			: Weapon(InWeapon), Start(InStart), End(InEnd) {}

		~FLastCache() 
		{
			Weapon->LastHitStart = Start;
			Weapon->LastHitEnd = End;
		}
		AWeapon* Weapon = nullptr;
		FVector Start, End;
	}; FLastCache LastCache(this, HitStart, HitEnd);

	FHitResult HitResult;
	//ĸ�� �õ� ��
	float CapsuleRadius = 50.0f;
	float CapsuleHalfHeight = 100.0f;

	TArray<AActor*> IgnoreActors;

	if (Owner)
	{
		IgnoreActors.Add(Owner->GetPawn()); //when die can't find pawn

		if (AMnBCharacter* Char = Cast<AMnBCharacter>(Owner->GetPawn()))
		{
			if (Char->GetEquippeddShield())
			{
				IgnoreActors.Add(Char->GetEquippeddShield());
			}
		}
	}
	IgnoreActors.Add(this);
	
	//UKismetSystemLibrary::SphereTraceSingleByProfile(GetWorld(), HitStart, HitEnd, 5.f, TEXT("Hitable"), false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Green);
	//UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), HitStart, HitEnd, 5.f, 5.f,ETraceTypeQuery::TraceTypeQuery4, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Green);
	//GetWorld()->SweepSingleByChannel(HitResult, HitStart, HitEnd, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel4, FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight));
	//DrawDebugCapsule(GetWorld(), HitResult.Location, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, FColor::Green, false, 2.f);
	//DrawDebugCylinder(GetWorld(), HitStart, HitEnd, 5.f, 1, FColor::Green,false, 2.f);
	//DrawDebugLine(GetWorld(), HitStart, HitEnd, FColor::Green, false, 2.f);
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), HitStart, HitEnd, 5.f, TraceTypeQuery4, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Green, FLinearColor::Red, 0.5f);
	if (!bHit)
	{
		const FVector CenterStart = (LastHitStart + LastHitEnd) / 2.0;
		const FVector CenterEnd = (HitStart + HitEnd) / 2.0;
		FVector Dir = (HitEnd - HitStart);
		Dir.Normalize();

		double Distance = UKismetMathLibrary::Vector_Distance(HitStart, HitEnd) / 2.0;
		bHit = UKismetSystemLibrary::BoxTraceSingle(GetWorld(), CenterStart, CenterEnd, 
			FVector(Distance, 5.f, 5.f), Dir.Rotation(), TraceTypeQuery4, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Green, FLinearColor::Red, 0.5f);
	}

	if (bHit)
	{
		bHitVRMode = true;
		HitDetectImpl(HitResult);
	}

	if (bApplyDamage == false && !bHit)
	{
		ObstacleDitect();
	}
}

void AWeapon::HitDetectImpl(FHitResult& InHitResult)
{
	FVector HitStart = StaticMeshComponent->GetSocketLocation(TEXT("HitStart"));
	FVector HitEnd = StaticMeshComponent->GetSocketLocation(TEXT("HitEnd"));

	/*if (ACharacter* Character = Cast<ACharacter>(InHitResult.GetActor()))
	{
		if (Character->GetController() == Owner)
		{
			return;
		}
	}*/

	/*if (AWeapon* HitWeapon = Cast<AWeapon>(InHitResult.GetActor()))
	{
		if (HitWeapon->Owner == Owner)
		{
			return;
		}
	}*/

	DrawDebugLine(
		GetWorld(),
		HitStart,
		InHitResult.Location,
		FColor::Red,
		false, 10.0f, 0, 1.0f
	);


	DrawDebugSphere(
		GetWorld(),
		InHitResult.Location,
		2.0f,
		24,
		FColor::Red,
		false, 10.0f
	);

	if (AWeapon* HitWeapon = Cast<AWeapon>(InHitResult.GetActor()))
	{
		if (Owner)
		{
			Owner->GetCharacter()->StopAnimMontage();
		}
		ParticleComponent->SetTemplate(Particles[1]);
		ParticleComponent->SetWorldLocation(InHitResult.ImpactPoint);
		ParticleComponent->ActivateSystem();

		SetRandomSoundAndPlay();

		return;
	}


	if (AShield* HitShield = Cast<AShield>(InHitResult.GetActor()))
	{
		AMnBCharacter* MnBCharacter = Cast<AMnBCharacter>(Owner->GetCharacter());
		if (MnBCharacter)
		{
			if (Owner)
			{
				Owner->GetCharacter()->StopAnimMontage();
				MnBCharacter->Blocked();

				ParticleComponent->SetTemplate(Particles[1]);
				ParticleComponent->SetWorldLocation(InHitResult.ImpactPoint);
				ParticleComponent->ActivateSystem();

				SetRandomSoundAndPlay();

				return;
			}

		}
	}

	ParticleComponent->SetTemplate(Particles[0]);
	ParticleComponent->SetWorldLocation(InHitResult.ImpactPoint);
	ParticleComponent->ActivateSystem();

	UGameplayStatics::ApplyDamage(InHitResult.GetActor(), 1, Owner, this, nullptr);

	bApplyDamage = true;
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

		bHitVRMode = true;

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

		SetRandomSoundAndPlay();

	}
	
	return bHit;
}

#include "Kismet/KismetMathLibrary.h"
void AWeapon::SetRandomSoundAndPlay()
{
	if (MetalSounds.Num() == 0) return;

	int rand = UKismetMathLibrary::RandomInteger(MetalSounds.Num());

	Audio->SetSound(MetalSounds[rand]);
	Audio->Play();
}

void AWeapon::Unequipped()
{
	if (Owner == nullptr) return;

	Owner = nullptr;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	StaticMeshComponent->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
	StaticMeshComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetWorldTimerManager().ClearTimer(Timer);

	bOwnerVR = false;
}
