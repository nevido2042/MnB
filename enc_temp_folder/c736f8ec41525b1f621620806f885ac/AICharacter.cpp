// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacter.h"
#include "Weapons/Weapon.h"
#include "Components/Health.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//{
	//	ConstructorHelpers::FClassFinder<AWeapon>Finder(TEXT("/Script/Engine.Blueprint'/Game/MyAssets/Weapons/BP_Sword.BP_Sword_C'"));
	//	if (Finder.Class)
	//	{
	//		WeaponAsset = Finder.Class;
	//	}
	//}

	
	{
		static ConstructorHelpers::FObjectFinder<UAnimMontage>Finder(TEXT("/Script/Engine.AnimMontage'/Game/MyAssets/Animations/Korea/Outward_Montage.Outward_Montage'"));
		if (Finder.Object)
		{
			LeftAttack = Finder.Object;
		}
	}

	{
		static ConstructorHelpers::FObjectFinder<UAnimMontage>Finder(TEXT("/Script/Engine.AnimMontage'/Game/MyAssets/Animations/Korea/Stable_Sword_Inward_Slash_Montage.Stable_Sword_Inward_Slash_Montage'"));
		if (Finder.Object)
		{
			RightAttack = Finder.Object;
		}
	}

	{
		static ConstructorHelpers::FObjectFinder<UAnimMontage>Finder(TEXT("/Script/Engine.AnimMontage'/Game/MyAssets/Animations/Korea/Standing_React_Large_From_Right_Montage.Standing_React_Large_From_Right_Montage'"));
		if (Finder.Object)
		{
			DamagedMontage = Finder.Object;
		}
	}

	Health = CreateDefaultSubobject<UHealth>(TEXT("Health"));


	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	Audio->SetupAttachment(RootComponent);

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionProfileName(TEXT("Hitable"));
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	EquipWeapon();
}

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* AAICharacter::GetBehaviorTree() const
{
	return Tree;
}

APatrolPath* AAICharacter::GetPatrolPath() const
{
	return PatrolPath;
}

UAnimMontage* AAICharacter::GetMontage() const
{
	return Montage;
}

int AAICharacter::MeleeAttack_Implementation()
{
	if (Montage)
	{
		PlayAnimMontage(Montage);
	}

	return 0;
}

void AAICharacter::EquipWeapon()
{
	AWeapon* NewWeapon = Cast<AWeapon>(GetWorld()->SpawnActor(WeaponAsset));
	if (NewWeapon == nullptr) return;

	if (GetController())
	{
		NewWeapon->Equipped(GetController());
	}

	UStaticMeshComponent* WeaponMesh = NewWeapon->GetComponentByClass<UStaticMeshComponent>();
	if (WeaponMesh == nullptr) return;
	
	bool result = false;
	auto mesh = GetMesh();
	result = WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("WeaponSocket"));

	if (result == false) return;

	WeaponMesh->SetRelativeLocation(FVector::Zero());
	WeaponMesh->SetRelativeRotation(NewWeapon->GetWeaponGrip()->GetRelativeRotation());

	CurWeapon = NewWeapon;
}

void AAICharacter::PlayAttack(bool bLeft)
{
	if (bLeft)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(LeftAttack);
	}
	else
	{
		GetMesh()->GetAnimInstance()->Montage_Play(RightAttack);
	}
}

float AAICharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health->AddCurrentHP(-Damage);

	SetRandomSoundAndPlay();

	StopAnimMontage();
	GetMesh()->GetAnimInstance()->Montage_Play(DamagedMontage);

	if (FMath::IsNearlyZero(Health->GetCurrentHP()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Die"));
		Die();
	}

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

#include "AI/MnBAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
void AAICharacter::Die()
{
	if (CurWeapon)
	{

		auto Childrens = GetMesh()->GetAttachChildren();
		for (auto Iter : Childrens)
		{
			if (Cast<AWeapon>(Iter))
			{
				Iter->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			}
		}

		CurWeapon->Unequipped();
		CurWeapon = nullptr;
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (UBlackboardComponent* BB = Cast<AMnBAIController>(GetController())->GetBlackboardComponent())
	{
		BB->SetValueAsBool("IsDie", true);
	}

	UnPossessed();
	Controller = nullptr;
	Tree = nullptr;

	bDie = true;

	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, this, &AAICharacter::MyDestroy, 3.0f, false);

	GetMesh()->SetCollisionProfileName("Dead");
	GetMesh()->SetSimulatePhysics(true);
}

void AAICharacter::MyDestroy()
{
	auto Childrens = GetMesh()->GetAttachChildren();
	for (auto Iter : Childrens)
	{
		Iter->DestroyComponent();
	}

	Destroy(); 
}

void AAICharacter::SetRandomSoundAndPlay()
{
	if (HitSounds.Num() == 0) return;

	int rand = UKismetMathLibrary::RandomInteger(HitSounds.Num());

	Audio->SetSound(HitSounds[rand]);
	Audio->Play();
}

