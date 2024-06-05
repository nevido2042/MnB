// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"

#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "VRHandSkeletalMeshComponent.h"
#include "VRHandAnimInstance.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Data/Input/BasicInputDataConfig.h"
#include "VRHandAnimInstance.h"
#include "InputActionValue.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapons/Weapon.h"

// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	VRCamera->SetupAttachment(RootComponent);

	MotionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerLeft"));
	MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerRight"));

	MotionControllerLeft->SetupAttachment(RootComponent);
	MotionControllerRight->SetupAttachment(RootComponent);

	MotionControllerLeft->SetTrackingMotionSource(LeftGrip);
	MotionControllerRight->SetTrackingMotionSource(RightGrip);

	HandGraphLeft = NewObject<UHandGraph>(this, TEXT("HandGraphLeft"));
	HandGraphRight = NewObject<UHandGraph>(this, TEXT("HandGraphRight"));

	GetMesh()->DestroyComponent();

	LeftHand = CreateDefaultSubobject<UVRHandSkeletalMeshComponent>(TEXT("LeftHand"));
	RightHand = CreateDefaultSubobject<UVRHandSkeletalMeshComponent>(TEXT("RightHand"));

	LeftHand->SetupAttachment(MotionControllerLeft);
	RightHand->SetupAttachment(MotionControllerRight);

	{
		ConstructorHelpers::FObjectFinder<USkeletalMesh>Finder(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
		RightHand->SetSkeletalMesh(Finder.Object);

		const FTransform Transform = FTransform(FRotator(25.000000, 0.000000, 89.999999), FVector(-2.981260, 3.500000, 4.561753));
		RightHand->SetRelativeTransform(Transform);
	}
	{
		ConstructorHelpers::FObjectFinder<USkeletalMesh>Finder(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
		LeftHand->SetSkeletalMesh(Finder.Object);

		const FTransform Transform = FTransform(FRotator(-25.000000, -179.999999, 89.999998), FVector(-2.981260, -3.500000, 4.561753));
		LeftHand->SetRelativeTransform(Transform);

		LeftHand->bMirror = true;
	}
	{
		ConstructorHelpers::FClassFinder<UVRHandAnimInstance>Finder(TEXT("/Script/Engine.AnimBlueprint'/Game/MyAssets/VR/ABP_Hand.ABP_Hand_C'"));
		ensure(Finder.Class);
		if (Finder.Class)
		{
			RightHand->SetAnimClass(Finder.Class);
			LeftHand->SetAnimClass(Finder.Class);
		}
	}

}	 

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			const UBasicInputDataConfig* BasicInputDataConfig = GetDefault<UBasicInputDataConfig>();
			Subsystem->AddMappingContext(BasicInputDataConfig->InputMappingContext, 0);

			const UVRHandsInputDataConfig* VRHandsInputDataConfig = GetDefault<UVRHandsInputDataConfig>();
			Subsystem->AddMappingContext(VRHandsInputDataConfig->InputMappingContext, 0);

			// HandGraphLeft, HandGraphRight
			const UVRHandsAnimationInputDataConfig* VRHandsAnimationInputDataConfig = GetDefault<UVRHandsAnimationInputDataConfig>();
			Subsystem->AddMappingContext(VRHandsAnimationInputDataConfig->InputMappingContext, 1);
		}
	}
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandRayCast(LeftHand);
	HandRayCast(RightHand);

}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		{
			const UBasicInputDataConfig* BasicInputDataConfig = GetDefault<UBasicInputDataConfig>();
			EnhancedInputComponent->BindAction(BasicInputDataConfig->Move, ETriggerEvent::Triggered, this, &ThisClass::OnMove);
		}

		{
			HandGraphLeft->SetupPlayerInputComponent(MotionControllerLeft, EnhancedInputComponent);
			HandGraphRight->SetupPlayerInputComponent(MotionControllerRight, EnhancedInputComponent);
		}

		{
			const UVRHandsInputDataConfig* VRHandsInputDataConfig = GetDefault<UVRHandsInputDataConfig>();
			EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_Grab_Left, ETriggerEvent::Started, this, &ThisClass::OnGrabLeftStarted);
			EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_Grab_Left, ETriggerEvent::Completed, this, &ThisClass::OnGrabLeftCompleted);

			EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_Grab_Right, ETriggerEvent::Started, this, &ThisClass::OnGrabRightStarted);
			EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_Grab_Right, ETriggerEvent::Completed, this, &ThisClass::OnGrabRightCompleted);
		}

	}


}

void AVRCharacter::OnMove(const FInputActionValue& InputActionValue)
{
	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

	const FRotator CameraRotator = VRCamera->GetRelativeRotation();
	const FRotator CameraYawRotator = FRotator(0., CameraRotator.Yaw, 0.);

	if (!FMath::IsNearlyZero(ActionValue.Y))
	{
		const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(CameraYawRotator);
		AddMovementInput(ForwardVector, ActionValue.Y);
	}

	if (!FMath::IsNearlyZero(ActionValue.X))
	{
		const FVector RightVector = UKismetMathLibrary::GetRightVector(CameraYawRotator);
		AddMovementInput(RightVector, ActionValue.X);
	}
}

void AVRCharacter::OnGrabStarted(UMotionControllerComponent* MotionControllerComponent, const bool bLeft, const FInputActionValue& InputActionValue)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, TEXT("OnGrabStarted"));

	if (bLeft)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, TEXT("Left"));
		Interact(LeftFocusingActor);
		Equip(LeftFocusingActor);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, TEXT("Right"));
		Interact(RightFocusingActor);
		Equip(RightFocusingActor);
	}
}

void AVRCharacter::OnGrabCompleted(UMotionControllerComponent* MotionControllerComponent, const bool bLeft, const FInputActionValue& InputActionValue)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, TEXT("OnGrabCompleted"));

	UnEquip();
}

void AVRCharacter::HandRayCast(UVRHandSkeletalMeshComponent* Hand)
{
	float LineLength = 500.f;
	FVector LineStart = Hand->GetComponentLocation();
	FVector LineEnd = LineStart + Hand->GetComponentRotation().RotateVector(FVector::RightVector) * LineLength;
	DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Green);

	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, LineStart, LineEnd, ECollisionChannel::ECC_GameTraceChannel3);
	if (bHit)
	{
		if (Hand == LeftHand)
		{
			LeftFocusingActor = HitResult.GetActor();
		}
		else if (Hand == RightHand)
		{
			RightFocusingActor = HitResult.GetActor();
		}
		


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
		if (Hand == LeftHand)
		{
			LeftFocusingActor = nullptr;
		}
		else if (Hand == RightHand)
		{
			RightFocusingActor = nullptr;
		}
	}
}

void AVRCharacter::Equip(AActor * HandFoucsing)
{
	const USkeletalMeshSocket* WeaponSocket = nullptr;
	UVRHandSkeletalMeshComponent* Hand = nullptr;
	AWeapon* EquippedWeapon = nullptr;
	FRotator RotateOffset = FRotator::ZeroRotator;

	if (HandFoucsing == RightFocusingActor)
	{
		WeaponSocket = RightHand->GetSocketByName("WeaponSocket");
		Hand = RightHand;
		EquippedWeapon = RightEquippedWeapon;
		RotateOffset = FRotator(90, 0, 180);
	}
	else if (HandFoucsing == LeftFocusingActor)
	{
		WeaponSocket = LeftHand->GetSocketByName("WeaponSocket");
		Hand = LeftHand;
		EquippedWeapon = LeftEquippedWeapon;
		RotateOffset = FRotator(-90, 180, 180);
	}
	
	if (WeaponSocket)
	{
		if (HandFoucsing == nullptr) { return; }

		if (EquippedWeapon != nullptr)
		{
			//EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			UStaticMeshComponent* WeaponMesh = EquippedWeapon->GetComponentByClass<UStaticMeshComponent>();
			WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

			EquippedWeapon->Unequipped();// 왼/ 오른 구분해야함
		}

		EquippedWeapon = Cast<AWeapon>(HandFoucsing);

		UStaticMeshComponent* WeaponMesh = HandFoucsing->GetComponentByClass<UStaticMeshComponent>();
		WeaponMesh->AttachToComponent(Hand, FAttachmentTransformRules::KeepWorldTransform, TEXT("WeaponSocket"));
		WeaponMesh->SetRelativeLocation(FVector::Zero());

		WeaponMesh->SetRelativeRotation(EquippedWeapon->GetWeaponGrip()->GetRelativeRotation()/*+FRotator(90, 0, 180)*/); //무기별 그립의 로테이션이 다른 것 때문에 문제

		//FocusingActor->AttachToComponent(WeaponScene,FAttachmentTransformRules::KeepWorldTransform,)

		/*bool bResult = WeaponSocket->AttachActor(FocusingActor, GetMesh());
		check(bResult);*/
	}
}

void AVRCharacter::UnEquip()
{
}

void AVRCharacter::Interact(AActor* HandFoucsing)
{
	if (HandFoucsing == nullptr) { return; }

	IInteractableActor* InteractableActor = Cast<IInteractableActor>(HandFoucsing);
	if (InteractableActor)
	{
		InteractableActor->Interact();
	}
}

