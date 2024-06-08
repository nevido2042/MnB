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

void AMnBCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CameraRaycast();
}

void AMnBCharacter::Equip()
{
	const USkeletalMeshSocket* WeaponSocket = GetMesh()->GetSocketByName("WeaponSocket");
	if (WeaponSocket)
	{
		if (FocusingActor == nullptr) { return; }

		if (EquippedWeapon != nullptr)
		{
			//EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			UStaticMeshComponent* WeaponMesh = EquippedWeapon->GetComponentByClass<UStaticMeshComponent>();
			WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

			EquippedWeapon->Unequipped();
		}

		EquippedWeapon = Cast<AWeapon>(FocusingActor);

		UStaticMeshComponent* WeaponMesh = FocusingActor->GetComponentByClass<UStaticMeshComponent>();
		WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("WeaponSocket"));
		WeaponMesh->SetRelativeLocation(FVector::Zero());
		WeaponMesh->SetRelativeRotation(EquippedWeapon->GetWeaponGrip()->GetRelativeRotation()); //무기별 로테이터 바꿔줘야함
		//FocusingActor->AttachToComponent(WeaponScene,FAttachmentTransformRules::KeepWorldTransform,)

		/*bool bResult = WeaponSocket->AttachActor(FocusingActor, GetMesh());
		check(bResult);*/
	}

}

void AMnBCharacter::ReadyToAttack()
{
	bReadyToLeftAttack = false;
	bReadyToRightAttack = false;

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
	if (bReadyToRightAttack)
	{
		bReadyToRightAttack = false;
		SetCurrentAttackDirection(EAttackDirection::AttackRight);
		PlayAnimMontage(AttackRightAnimMontage);
	}
	else if (bReadyToLeftAttack)
	{
		bReadyToLeftAttack = false;
		SetCurrentAttackDirection(EAttackDirection::AttackLeft);
		PlayAnimMontage(AttackLeftAnimMontage);
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
			false, 5.0f, 0, 1.0f
		);

		
		DrawDebugSphere(
			GetWorld(),
			HitResult.Location,
			12.0f,
			24,
			FColor::Red,
			false, 5.0f
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

	IInteractableActor* InteractableActor = Cast<IInteractableActor>(FocusingActor);
	if (InteractableActor)
	{
		InteractableActor->Interact();
	}
}

void AMnBCharacter::RecoverBlockedState()
{
	Cast<UMnBCharacterAnimInstance>(GetMesh()->GetAnimInstance())->SetBlockedDirection(EBlockedDirection::None);
}

void AMnBCharacter::Gaurd()
{
	UE_LOG(LogTemp, Warning, TEXT("Garud()"));
	if (LookAxisVector.X < 0)
	{
		Cast<UMnBCharacterAnimInstance>(GetMesh()->GetAnimInstance())->SetGaurdDirection(EGaurdDirection::GaurdLeft);
	}
	else
	{
		Cast<UMnBCharacterAnimInstance>(GetMesh()->GetAnimInstance())->SetGaurdDirection(EGaurdDirection::GarudRight);
	}
}

void AMnBCharacter::GaurdEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("GarudEnd()"));
	Cast<UMnBCharacterAnimInstance>(GetMesh()->GetAnimInstance())->SetGaurdDirection(EGaurdDirection::GaurdNone);
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

float AMnBCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health->AddCurrentHP(-Damage);

	if (FMath::IsNearlyZero(Health->GetCurrentHP()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Die"));
	}

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
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
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AMnBCharacter::ReadyToAttack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AMnBCharacter::Attack);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AMnBCharacter::Interact);

		// Gaurd
		EnhancedInputComponent->BindAction(GaurdAction, ETriggerEvent::Started, this, &AMnBCharacter::Gaurd);
		EnhancedInputComponent->BindAction(GaurdAction, ETriggerEvent::Completed, this, &AMnBCharacter::GaurdEnd);

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