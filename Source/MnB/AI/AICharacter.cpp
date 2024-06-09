// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacter.h"
#include "Weapons/Weapon.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	{
		ConstructorHelpers::FClassFinder<AWeapon>Finder(TEXT("/Script/Engine.Blueprint'/Game/MyAssets/Weapons/BP_Sword.BP_Sword_C'"));
		if (Finder.Class)
		{
			WeaponAsset = Finder.Class;
		}
	}

	
	{
		ConstructorHelpers::FObjectFinder<UAnimMontage>Finder(TEXT("/Script/Engine.AnimMontage'/Game/MyAssets/Animations/Korea/Outward_Montage.Outward_Montage'"));
		if (Finder.Object)
		{
			LeftAttack = Finder.Object;
		}
	}

	{
		ConstructorHelpers::FObjectFinder<UAnimMontage>Finder(TEXT("/Script/Engine.AnimMontage'/Game/MyAssets/Animations/Korea/Stable_Sword_Inward_Slash_Montage.Stable_Sword_Inward_Slash_Montage'"));
		if (Finder.Object)
		{
			RightAttack = Finder.Object;
		}
	}


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

	//{
	//	auto const* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	//	auto* const Cont = GetController();
	//	auto* const Pawn = Cont->GetPawn();

	//	const FVector MyLocation = Cont->GetPawn()->GetActorLocation();
	//	const FVector TargetLocation = Player->GetActorLocation(); //���߿� �÷��̾�� Ÿ������ �ٲ㺸��

	//	FRotator NewControlRotation = UKismetMathLibrary::MakeRotFromX((TargetLocation - MyLocation).GetSafeNormal());
	//	//FRotator New = FRotator(Pawn->GetActorRotation().Pitch, NewControlRotation.Yaw, Pawn->GetActorRotation().Roll);
	//	FRotator NewRotation = FRotator(0.f, NewControlRotation.Yaw, NewControlRotation.Roll);

	//	Cont->GetPawn()->SetActorRotation(NewRotation);

	//	//UE_LOG(LogTemp, Display, TEXT("Pitch:%f Yaw:%f Roll:%f"), NewControlRotation.Pitch, NewControlRotation.Yaw, NewControlRotation.Roll);
	//	//Cont->SetControlRotation(New);
	//}
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

	NewWeapon->Equipped(GetController());

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

