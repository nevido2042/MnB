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
#include "Weapons/Arrow.h"
#include "AI/AICharacter.h"
#include "Enum/Enums.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NPC/ArenaManager.h"
#include "UserWidget/InventoryUserWidget.h"
#include "AI/MnBAIController.h"
#include "Weapons/Shield.h"
#include "Weapons/Bow.h"
#include "Controller/ControllerPC.h"
#include "MnB/UserWidget/PCWidget.h"
#include "UserWidget/ExitTownWidget.h"
#include "Components/ProgressBar.h"
#include "AI/InfantryAI.h"
#include "AI/BowManAI.h"
#include "UserWidget/ControlUnitWidget.h"



//DEFINE_LOG_CATEGORY(LogTemplateCharacter);

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

	/*ExitTownWidget = CreateWidget(ExitTownWidgetAsset);
	ExitTownWidget->AddToViewport();
	ExitTownWidget->SetVisibility(ESlateVisibility::Hidden);*/
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
	UpdateActorInfo();

	if (bControlUnits)
	{
		ControlUnitRaycast();
	}

	//if (bHorseSlowDown)
	//{
	//	HorseSlowDown();
	//}
}


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

		if (Cast<ABow>(EquippedWeapon))
		{
			UStaticMeshComponent* WeaponMesh = EquippedWeapon->GetComponentByClass<UStaticMeshComponent>();
			WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

			EquippedWeapon->Unequipped();
			EquippedWeapon = nullptr;
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

		if (EquippedShield)
		{
			UStaticMeshComponent* WeaponMesh = EquippedShield->GetComponentByClass<UStaticMeshComponent>();
			WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

			EquippedShield->Unequipped();
			EquippedShield = nullptr;
		}

		UStaticMeshComponent* WeaponMesh = FocusingActor->GetComponentByClass<UStaticMeshComponent>();

		WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("BowSocket"));
		EquippedWeapon = Cast<ABow>(FocusingActor);

		WeaponMesh->SetRelativeLocation(FVector::Zero());
		WeaponMesh->SetRelativeRotation(EquippedWeapon->GetWeaponGrip()->GetRelativeRotation());

		SetBow(true);
	}
	else
	{
		if (EquippedWeapon != nullptr)
		{
			UStaticMeshComponent* WeaponMesh = EquippedWeapon->GetComponentByClass<UStaticMeshComponent>();
			WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

			EquippedWeapon->Unequipped();
			SetBow(false);
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
	if (bControlUnits)
	{
		//for (AAICharacter* AICharacter : CallUnits)
		//{
		//	if (AICharacter->IsDie()||AICharacter== nullptr)
		//	{
		//		continue;
		//	}

		//	Cast<AAIController>(AICharacter->GetController())->MoveToLocation(ControlHitResult.Location);
		//	Cast<AAIController>(AICharacter->GetController())->GetBlackboardComponent()->SetValueAsBool("IsCharge", false);
		//}

		//CancelControl();

		return;
	}

	//bReadyToAttack = true;

	bReadyToLeftAttack = false;
	bReadyToRightAttack = false;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance->IsAnyMontagePlaying()) { return; }

	if (bBow)
	{
		UMnBCharacterAnimInstance* MnBAnim = Cast<UMnBCharacterAnimInstance>(AnimInstance);
		MnBAnim->SetBowDraw(true);
		return;
	}

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
	if (bControlUnits)
	{
		for (AAICharacter* AICharacter : CallUnits)
		{
			if (AICharacter->IsDie() || AICharacter == nullptr)
			{
				continue;
			}

			Cast<AAIController>(AICharacter->GetController())->MoveToLocation(ControlHitResult.Location);
			Cast<AAIController>(AICharacter->GetController())->GetBlackboardComponent()->SetValueAsBool("IsCharge", false);
		}

		CancelControl();

		return;
	}

	//bReadyToAttack = false;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance->IsAnyMontagePlaying()) { return; }

	if (bBow)
	{
		UMnBCharacterAnimInstance* MnBAnim = Cast<UMnBCharacterAnimInstance>(AnimInstance);
		MnBAnim->SetBowDraw(false);

		PlayAnimMontage(BowRecoil);

		FVector Location = GetMesh()->GetSocketByName("WeaponSocket")->GetSocketLocation(GetMesh());
		FRotator Rotation = GetControlRotation();
		GetWorld()->SpawnActor(Arrow, &Location, &Rotation);

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

void AMnBCharacter::ControlUnitRaycast()
{
	float LineLength = 5000.f;
	FVector LineStart = GetFollowCamera()->GetComponentLocation();
	FVector LineEnd = LineStart + GetFollowCamera()->GetComponentRotation().Vector() * LineLength;
	DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Green);

	ControlHitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(ControlHitResult, LineStart, LineEnd, ECollisionChannel::ECC_GameTraceChannel5);//Ground
	if (bHit)
	{
		DrawDebugLine(
			GetWorld(),
			LineStart,
			ControlHitResult.Location,
			FColor::Red,
			false, 0.5f, 0, 1.0f
		);


		DrawDebugSphere(
			GetWorld(),
			ControlHitResult.Location,
			12.0f,
			24,
			FColor::Red,
			false, 0.5f
		);

		int Index = 1;
		int Scale = 100;
		for (AActor* Flag : Flags)
		{
			Flag->SetActorLocation(ControlHitResult.Location);
			Flag->AddActorLocalOffset(FVector::ForwardVector * Index * Scale);
			Index++;
		}
		
		//CurFlag->SetActorLocation(ControlHitResult.Location);
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

		if (OutActors.IsEmpty())
		{
			return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		}

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
	float RotateScale = 2.f;

	CurHorse->AddMovementInput(CurHorse->GetActorForwardVector(), Vect.Y);
	CurHorse->AddActorLocalRotation(FRotator(0.f, Vect.X * RotateScale, 0.f));
}

void AMnBCharacter::SetBow(bool Value)
{
	bBow = Value;
	UMnBCharacterAnimInstance* Anim = Cast<UMnBCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	Anim->SetBow(Value);
}

void AMnBCharacter::SetVisibleExitTownWidget()
{
	ExitTownWidget = Cast<UExitTownWidget>(Cast<AControllerPC>(GetController())->GetExitTownWidget());
	ExitTownWidget->SetVisibility(ESlateVisibility::Visible);
}

void AMnBCharacter::SetHiddenExitTownWidget()
{
	ExitTownWidget->SetVisibility(ESlateVisibility::Hidden);
	ExitTownWidget->SetCurPercent(0.f);
	ExitTownWidget->GetExitTownBar()->SetPercent(0.f);
}

void AMnBCharacter::ExitTownIncreasePercent()
{
	//ExitTownWidget->SetVisibility(ESlateVisibility::Visible);
	/*UE_LOG(LogTemp, Warning, TEXT("ExitTownIncreasePercent"));*/
	
	float NewPercent = ExitTownWidget->GetCurPercent() + GetWorld()->DeltaTimeSeconds;
	//NewPercent = NewPercent / 10.f;
	ExitTownWidget->SetCurPercent(NewPercent);
	ExitTownWidget->GetExitTownBar()->SetPercent(NewPercent);

	if (ExitTownWidget->GetCurPercent() > 1.f)
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("WorldLevel"));
	}
}

void AMnBCharacter::SpawnFlags()
{
	for (int i = 0; i < 5; i++)
	{
		Flags.Add(GetWorld()->SpawnActor(FlagAsset));
	}
}

void AMnBCharacter::DestroyFlags()
{
	for (AActor* Flag : Flags)
	{
		Flag->Destroy();
	}

	Flags.Empty();
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
		//EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMnBCharacter::StartHorseSlowDown);

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

		//ExitTown
		EnhancedInputComponent->BindAction(ExitTownAction, ETriggerEvent::Started, this, &AMnBCharacter::SetVisibleExitTownWidget);
		EnhancedInputComponent->BindAction(ExitTownAction, ETriggerEvent::Triggered, this, &AMnBCharacter::ExitTownIncreasePercent);
		EnhancedInputComponent->BindAction(ExitTownAction, ETriggerEvent::Completed, this, &AMnBCharacter::SetHiddenExitTownWidget);
		EnhancedInputComponent->BindAction(ExitTownAction, ETriggerEvent::Canceled, this, &AMnBCharacter::SetHiddenExitTownWidget);

		//Call All
		EnhancedInputComponent->BindAction(CallAllAction, ETriggerEvent::Completed, this, &AMnBCharacter::CallAll);

		//Call Infantry
		EnhancedInputComponent->BindAction(CallInfantryAction, ETriggerEvent::Completed, this, &AMnBCharacter::CallInfantry);

		//Call BowMan
		EnhancedInputComponent->BindAction(CallBowManAction, ETriggerEvent::Completed, this, &AMnBCharacter::CallBowMan);


		//Charge
		EnhancedInputComponent->BindAction(ChargeAction, ETriggerEvent::Started, this, &AMnBCharacter::Charge);

		//Stop
		EnhancedInputComponent->BindAction(StopAction, ETriggerEvent::Started, this, &AMnBCharacter::Stop);

	}
	/*else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}*/
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
			//bHorseSlowDown = false;

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

void AMnBCharacter::CancelControl()
{
	bControlUnits = false;

	//CurFlag->Destroy();
	DestroyFlags();


	CallUnits.Empty();

	Cast<UControlUnitWidget>(Cast<AControllerPC>(GetController())->GetControlUnitWidget())->ActiveSwordManPanel(false);
	Cast<UControlUnitWidget>(Cast<AControllerPC>(GetController())->GetControlUnitWidget())->ActiveBowManPanel(false);
}

#include "AI/AICharacter.h"
void AMnBCharacter::CallAll()
{
	if (bControlUnits == true)
	{
		CancelControl();
		return;
	}

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAICharacter::StaticClass(), OutActors);

	uint8 SwordManCnt = 0;
	uint8 BowManCnt = 0;

	for (AActor* Iter : OutActors)
	{
		AAICharacter* AICharacter = Cast<AAICharacter>(Iter);
		if (AICharacter->IsDie())
		{
			continue;
		}

		if (AICharacter->GetTeam() == ETeam::ATeam)
		{
			CallUnits.Add(AICharacter);

			if (Cast<AInfantryAI>(AICharacter))
			{
				++SwordManCnt;
			}
			else if (Cast<ABowManAI>(AICharacter))
			{
				++BowManCnt;
			}
		}
	}

	bControlUnits = true;

	SpawnFlags();
	//CurFlag = GetWorld()->SpawnActor(FlagAsset);


	Cast<UControlUnitWidget>(Cast<AControllerPC>(GetController())->GetControlUnitWidget())->ActiveSwordManPanel(true, SwordManCnt);
	Cast<UControlUnitWidget>(Cast<AControllerPC>(GetController())->GetControlUnitWidget())->ActiveBowManPanel(true, BowManCnt);
}

void AMnBCharacter::CallInfantry()
{
	if (bControlUnits == true)
	{
		CancelControl();
		return;
	}

	TArray<AActor*> OutActors;
	//Infantry만 체크
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInfantryAI::StaticClass(), OutActors);

	uint8 SwordManCnt = 0;

	for (AActor* Iter : OutActors)
	{
		AAICharacter* AICharacter = Cast<AAICharacter>(Iter);
		if (AICharacter->IsDie())
		{
			continue;
		}

		if (AICharacter->GetTeam() == ETeam::ATeam)
		{
			CallUnits.Add(AICharacter);

			if (Cast<AInfantryAI>(AICharacter))
			{
				++SwordManCnt;
			}
		}
	}

	bControlUnits = true;
	//CurFlag = GetWorld()->SpawnActor(FlagAsset);
	SpawnFlags();

	Cast<UControlUnitWidget>(Cast<AControllerPC>(GetController())->GetControlUnitWidget())->ActiveSwordManPanel(true, SwordManCnt);

}

void AMnBCharacter::CallBowMan()
{
	if (bControlUnits == true)
	{
		CancelControl();
		return;
	}

	TArray<AActor*> OutActors;
	//BowMan만 체크
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABowManAI::StaticClass(), OutActors);

	uint8 BowManCnt = 0;

	for (AActor* Iter : OutActors)
	{
		AAICharacter* AICharacter = Cast<AAICharacter>(Iter);

		if (AICharacter->IsDie())
		{
			continue;
		}

		if (AICharacter->GetTeam() == ETeam::ATeam)
		{
			CallUnits.Add(AICharacter);

			if (Cast<ABowManAI>(AICharacter))
			{
				++BowManCnt;
			}
		}
	}

	bControlUnits = true;
	//CurFlag = GetWorld()->SpawnActor(FlagAsset);
	SpawnFlags();
	Cast<UControlUnitWidget>(Cast<AControllerPC>(GetController())->GetControlUnitWidget())->ActiveBowManPanel(true, BowManCnt);

}

void AMnBCharacter::Charge()
{
	if (bControlUnits)
	{
		for (AAICharacter* AICharacter : CallUnits)
		{
			if (AICharacter->IsDie())
			{
				continue;
			}

			Cast<AAIController>(AICharacter->GetController())->GetBlackboardComponent()->SetValueAsBool("IsCharge", true);
		}

		//CancelControl();
	}
}

void AMnBCharacter::Stop()
{
	if (bControlUnits)
	{
		for (AAICharacter* AICharacter : CallUnits)
		{
			if (AICharacter->IsDie())
			{
				continue;
			}

			Cast<AAIController>(AICharacter->GetController())->GetBlackboardComponent()->SetValueAsBool("IsCharge", false);
			Cast<AAIController>(AICharacter->GetController())->StopMovement();
		}

		//CancelControl();
	}
}
