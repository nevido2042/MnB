// Copyright Epic Games, Inc. All Rights Reserved.

#include "MnBCharacter.h"

#include "Perception/AISense_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/ChildActorComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "MnB/Interface/InteractableActor.h"
#include "MnB/Weapons/Weapon.h"
#include "MnB/AnimInstance/MnBCharacterAnimInstance.h"
#include "Components/Health.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Horse/Horse.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMnBCharacter

AMnBCharacter::AMnBCharacter()
{
	// Set size for collision capsule

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	SetupStimulusSource();

	Health = CreateDefaultSubobject<UHealth>(TEXT("Health"));

	{
		static ConstructorHelpers::FObjectFinder<UAnimMontage>Finder(TEXT("/Script/Engine.AnimMontage'/Game/MyAssets/Animations/Korea/Standing_React_Large_From_Right_Montage.Standing_React_Large_From_Right_Montage'"));
		ensure(Finder.Object);
		if (Finder.Object)
		{
			DamagedMontage = Finder.Object;
		}
	}

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	Audio->SetupAttachment(RootComponent);
}

void AMnBCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

#include "Controller/ControllerPC.h"
#include "MnB/UserWidget/PCWidget.h"
void AMnBCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CameraRaycast();
	UpdateActorInfo();

	/*if (bOnHorse)
	{
		FVector Loc = CurHorse->GetSitLocation()->GetComponentLocation();
		SetActorLocation(Loc);
	}*/
}

#include "Weapons/Shield.h"
#include "Weapons/Bow.h"
void AMnBCharacter::Equip()
{
	if (FocusingActor == nullptr) { return; }

	if (Cast<AShield>(FocusingActor))
	{
		if (EquippedShield != nullptr)
		{

			UStaticMeshComponent* WeaponMesh = EquippedShield->GetComponentByClass<UStaticMeshComponent>();
			WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

			EquippedShield->Unequipped();
		}

		UStaticMeshComponent* WeaponMesh = FocusingActor->GetComponentByClass<UStaticMeshComponent>();

		WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("ShieldSocket"));
		EquippedShield = Cast<AShield>(FocusingActor);

		WeaponMesh->SetRelativeLocation(FVector::Zero());
		WeaponMesh->SetRelativeRotation(EquippedShield->GetWeaponGrip()->GetRelativeRotation());
	}
	else if (Cast<ABow>(FocusingActor))
	{
		if (EquippedWeapon != nullptr)
		{

			UStaticMeshComponent* WeaponMesh = EquippedWeapon->GetComponentByClass<UStaticMeshComponent>();
			WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

			EquippedWeapon->Unequipped();
		}

		UStaticMeshComponent* WeaponMesh = FocusingActor->GetComponentByClass<UStaticMeshComponent>();

		WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("BowSocket"));
		EquippedWeapon = Cast<ABow>(FocusingActor);

		WeaponMesh->SetRelativeLocation(FVector::Zero());
		WeaponMesh->SetRelativeRotation(EquippedWeapon->GetWeaponGrip()->GetRelativeRotation());
	}
	else
	{
		if (EquippedWeapon != nullptr)
		{
			UStaticMeshComponent* WeaponMesh = EquippedWeapon->GetComponentByClass<UStaticMeshComponent>();
			WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

			EquippedWeapon->Unequipped();
		}

		UStaticMeshComponent* WeaponMesh = FocusingActor->GetComponentByClass<UStaticMeshComponent>();

		WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("WeaponSocket"));
		EquippedWeapon = Cast<AWeapon>(FocusingActor);

		WeaponMesh->SetRelativeLocation(FVector::Zero());
		WeaponMesh->SetRelativeRotation(EquippedWeapon->GetWeaponGrip()->GetRelativeRotation());
	}



}

void AMnBCharacter::ReadyToAttack()
{
	//bReadyToAttack = true;

	bReadyToLeftAttack = false;
	bReadyToRightAttack = false;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance->IsAnyMontagePlaying()) { return; }

	if (LookAxisVector.X < 0)
	{
		bReadyToLeftAttack = true;
	}
	else
	{
		bReadyToRightAttack = true;
	}

}

void AMnBCharacter::Attack()
{
	//bReadyToAttack = false;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance->IsAnyMontagePlaying()) { return; }

	if (bBow)
	{
		UMnBCharacterAnimInstance* MnBAnim = Cast<UMnBCharacterAnimInstance>(AnimInstance);
		MnBAnim->SetBowDraw(true);
		return;
	}

	if (bReadyToRightAttack)
	{
		SetCurrentAttackDirection(EAttackDirection::AttackRight);
		PlayAnimMontage(AttackRightAnimMontage, 1.5f);
		bReadyToRightAttack = false;
	}
	else if (bReadyToLeftAttack)
	{
		SetCurrentAttackDirection(EAttackDirection::AttackLeft);
		PlayAnimMontage(AttackLeftAnimMontage, 1.5f);
		bReadyToLeftAttack = false;
	}
}

void AMnBCharacter::CameraRaycast()
{
	float LineLength = 500.f;
	FVector LineStart = GetFollowCamera()->GetComponentLocation();
	FVector LineEnd = LineStart + GetFollowCamera()->GetComponentRotation().Vector() * LineLength;
	DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Green);

	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, LineStart, LineEnd, ECollisionChannel::ECC_GameTraceChannel3);
	if (bHit)
	{
		FocusingActor = HitResult.GetActor();

		
		DrawDebugLine(
			GetWorld(),
			LineStart,
			HitResult.Location,
			FColor::Red,
			false, 0.5f, 0, 1.0f
		);

		
		DrawDebugSphere(
			GetWorld(),
			HitResult.Location,
			12.0f,
			24,
			FColor::Red,
			false, 0.5f
		);
	}
	else
	{
		FocusingActor = nullptr;
	}
}

void AMnBCharacter::Interact()
{
	if (FocusingActor == nullptr) { return; }

	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		return;
	}

	IInteractableActor* InteractableActor = Cast<IInteractableActor>(FocusingActor);
	if (InteractableActor)
	{
		InteractableActor->Interact(this);
	}
}

void AMnBCharacter::RecoverBlockedState()
{
	Cast<UMnBCharacterAnimInstance>(GetMesh()->GetAnimInstance())->SetBlockedDirection(EBlockedDirection::None);
}

void AMnBCharacter::Guard()
{
	if (LookAxisVector.X < 0)
	{
		Cast<UMnBCharacterAnimInstance>(GetMesh()->GetAnimInstance())->SetGuardDirection(EGuardDirection::GuardLeft);
	}
	else
	{
		Cast<UMnBCharacterAnimInstance>(GetMesh()->GetAnimInstance())->SetGuardDirection(EGuardDirection::GarudRight);
	}

	if (EquippedWeapon == nullptr) return;
	if (EquippedWeapon->GetGuardCollider() == nullptr) return;//bow not has guard collider

	EquippedWeapon->GetGuardCollider()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMnBCharacter::GuardEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("GarudEnd()"));
	Cast<UMnBCharacterAnimInstance>(GetMesh()->GetAnimInstance())->SetGuardDirection(EGuardDirection::GuardNone);

	if (EquippedWeapon == nullptr) return;
	if (EquippedWeapon->GetGuardCollider() == nullptr) return;//bow not has guard collider

	EquippedWeapon->GetGuardCollider()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

#include "UserWidget/InventoryUserWidget.h"
void AMnBCharacter::InventoryOnOFF()
{
	if (UInventoryUserWidget* InvenWidget = Cast<AControllerPC>(GetController())->GetInventoryWidget())
	{
		APlayerController* PC = Cast<APlayerController>(GetController());

		if (InvenWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			InvenWidget->SetVisibility(ESlateVisibility::Hidden);

			PC->SetShowMouseCursor(false);

			FInputModeGameOnly Mode;
			PC->SetInputMode(Mode);
		}
		else
		{
			InvenWidget->SetVisibility(ESlateVisibility::Visible);

			PC->SetShowMouseCursor(true);

			FInputModeGameAndUI Mode;
			PC->SetInputMode(Mode);
		}
	}
}

void AMnBCharacter::Blocked()
{
	if (CurAttackDir == EAttackDirection::AttackLeft)
	{
		Cast<UMnBCharacterAnimInstance>(GetMesh()->GetAnimInstance())->SetBlockedDirection(EBlockedDirection::Left);
	}
	else
	{
		Cast<UMnBCharacterAnimInstance>(GetMesh()->GetAnimInstance())->SetBlockedDirection(EBlockedDirection::Right);
	}

	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, this, &AMnBCharacter::RecoverBlockedState, 0.5f);
}

void AMnBCharacter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

#include "NPC/ArenaManager.h"
float AMnBCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health->AddCurrentHP(-Damage);

	SetRandomSoundAndPlay();

	StopAnimMontage();
	GetMesh()->GetAnimInstance()->Montage_Play(DamagedMontage);

	if (FMath::IsNearlyZero(Health->GetCurrentHP()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Die"));
		
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArenaManager::StaticClass(), OutActors);

		if (OutActors[0])
		{
			if (AArenaManager * ArenaManager = Cast<AArenaManager>(OutActors[0]))
			{
				ArenaManager->RecoveryAndReturn();
			}

		}

	}

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void AMnBCharacter::UpdateActorInfo()
{
	if (AControllerPC* Cont = Cast<AControllerPC>(GetController()))
	{
		if (UPCWidget* PCWidget = Cast<UPCWidget>(Cont->GetCurrentWidget()))
		{
			if (FocusingActor)
			{
				PCWidget->HideInfo(false);

				if (IInteractableActor* InteractableActor = Cast<IInteractableActor>(FocusingActor))
				{
					PCWidget->SetActorInfo(InteractableActor->GetInfo());
				}
			}
			else
			{
				PCWidget->HideInfo(true);
			}
		}
	}
}

#include "Kismet/KismetMathLibrary.h"
void AMnBCharacter::SetRandomSoundAndPlay()
{
	int rand = UKismetMathLibrary::RandomInteger(HitSounds.Num());

	Audio->SetSound(HitSounds[rand]);
	Audio->Play();
}

void AMnBCharacter::StartDescendingHorseMontage()
{
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		return;
	}

	PlayAnimMontage(StartDescendingMontage);
}

void AMnBCharacter::MoveHorse(FVector2D Vect)
{
	CurHorse->AddMovementInput(CurHorse->GetActorForwardVector(), Vect.Y);
	CurHorse->AddActorLocalRotation(FRotator(0.f, Vect.X, 0.f));
}

void AMnBCharacter::StartGetOnMontage()
{
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		return;
	}

	PlayAnimMontage(StartGetOnHorseMontage);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMnBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMnBCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMnBCharacter::Look);

		// Attacking
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMnBCharacter::ReadyToAttack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AMnBCharacter::Attack);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AMnBCharacter::Interact);

		// Guard
		EnhancedInputComponent->BindAction(GuardAction, ETriggerEvent::Triggered, this, &AMnBCharacter::Guard);
		EnhancedInputComponent->BindAction(GuardAction, ETriggerEvent::Completed, this, &AMnBCharacter::GuardEnd);

		// Inventory
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Completed, this, &AMnBCharacter::InventoryOnOFF);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMnBCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		if (CurHorse)
		{
			MoveHorse(MovementVector);
		}
	}
}

void AMnBCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	LookAxisVector = Value.Get<FVector2D>();

	//UE_LOG(LogTemp, Warning, TEXT("%f"), LookAxisVector.X);

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}