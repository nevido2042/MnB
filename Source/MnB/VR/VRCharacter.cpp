// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"

#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "VRHandSkeletalMeshComponent.h"
#include "VRHandAnimInstance.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "VRHandAnimInstance.h"
#include "InputActionValue.h"

// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(RootComponent);

	MotionControllerComponentLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerComponentLeft"));
	MotionControllerComponentRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerComponentRight"));

	MotionControllerComponentLeft->SetupAttachment(RootComponent);
	MotionControllerComponentRight->SetupAttachment(RootComponent);

	MotionControllerComponentLeft->SetTrackingMotionSource((TEXT("LeftGrip")));
	MotionControllerComponentRight->SetTrackingMotionSource((TEXT("RightGrip")));

	GetMesh()->DestroyComponent();

	LeftHand = CreateDefaultSubobject<UVRHandSkeletalMeshComponent>(TEXT("LeftHand"));
	RightHand = CreateDefaultSubobject<UVRHandSkeletalMeshComponent>(TEXT("RightHand"));

	LeftHand->SetupAttachment(MotionControllerComponentLeft);
	RightHand->SetupAttachment(MotionControllerComponentRight);

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

	{
		ConstructorHelpers::FObjectFinder<UInputMappingContext>Finder(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/MyAssets/VR/Input/IMC_Hands.IMC_Hands'"));
		ensure(Finder.Object);
		if (Finder.Object)
		{
			MappingContext = Finder.Object;
		}
	}
	{
		ConstructorHelpers::FObjectFinder<UInputAction>Finder(TEXT("/Script/EnhancedInput.InputAction'/Game/MyAssets/VR/Input/IA_Grab_Right.IA_Grab_Right'"));
		ensure(Finder.Object);
		if (Finder.Object)
		{
			IA_GrabRight = Finder.Object;
		}
	}
	{
		ConstructorHelpers::FObjectFinder<UInputAction>Finder(TEXT("/Script/EnhancedInput.InputAction'/Game/MyAssets/VR/Input/IA_Grab_Left.IA_Grab_Left'"));
		ensure(Finder.Object);
		if (Finder.Object)
		{
			IA_GrabLeft = Finder.Object;
		}
	}
	{
		ConstructorHelpers::FObjectFinder<UInputAction>Finder(TEXT("/Script/EnhancedInput.InputAction'/Game/MyAssets/VR/Input/IA_Point_Right.IA_Point_Right'"));
		ensure(Finder.Object);
		if (Finder.Object)
		{
			IA_PointRight = Finder.Object;
		}
	}
	{
		ConstructorHelpers::FObjectFinder<UInputAction>Finder(TEXT("/Script/EnhancedInput.InputAction'/Game/MyAssets/VR/Input/IA_Point_Left.IA_Point_Left'"));
		ensure(Finder.Object);
		if (Finder.Object)
		{
			IA_PointLeft = Finder.Object;
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
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhancedInputComponent->BindAction(IA_GrabRight, ETriggerEvent::Triggered, this, &AVRCharacter::GrabRight);
		EnhancedInputComponent->BindAction(IA_GrabRight, ETriggerEvent::Canceled, this, &AVRCharacter::GrabRight);
		EnhancedInputComponent->BindAction(IA_GrabRight, ETriggerEvent::Completed, this, &AVRCharacter::GrabRight);

		EnhancedInputComponent->BindAction(IA_GrabLeft, ETriggerEvent::Triggered, this, &AVRCharacter::GrabLeft);
		EnhancedInputComponent->BindAction(IA_GrabLeft, ETriggerEvent::Canceled, this, &AVRCharacter::GrabLeft);
		EnhancedInputComponent->BindAction(IA_GrabLeft, ETriggerEvent::Completed, this, &AVRCharacter::GrabLeft);

		EnhancedInputComponent->BindAction(IA_PointRight, ETriggerEvent::Started, this, &AVRCharacter::PointRightTouch);
		EnhancedInputComponent->BindAction(IA_PointRight, ETriggerEvent::Canceled, this, &AVRCharacter::PointRightTouch);
		EnhancedInputComponent->BindAction(IA_PointRight, ETriggerEvent::Completed, this, &AVRCharacter::PointRightTouchEnd);

		EnhancedInputComponent->BindAction(IA_PointLeft, ETriggerEvent::Started, this, &AVRCharacter::PointLeftTouch);
		EnhancedInputComponent->BindAction(IA_PointLeft, ETriggerEvent::Canceled, this, &AVRCharacter::PointLeftTouch);
		EnhancedInputComponent->BindAction(IA_PointLeft, ETriggerEvent::Completed, this, &AVRCharacter::PointLeftTouchEnd);


	}

}

void AVRCharacter::GrabRight(const FInputActionValue& Value)
{
	if (UVRHandAnimInstance* Anim = Cast<UVRHandAnimInstance>(RightHand->GetAnimInstance()))
	{
		Anim->SetPoseAlphaGrap(Value.Get<float>());
	}
}

void AVRCharacter::GrabLeft(const FInputActionValue& Value)
{
	if (UVRHandAnimInstance* Anim = Cast<UVRHandAnimInstance>(LeftHand->GetAnimInstance()))
	{
		Anim->SetPoseAlphaGrap(Value.Get<float>());
	}
}

void AVRCharacter::PointRightTouch()
{
	if (UVRHandAnimInstance* Anim = Cast<UVRHandAnimInstance>(RightHand->GetAnimInstance()))
	{
		Anim->SetPoseAlphaPoint(0);
	}
}

void AVRCharacter::PointLeftTouch()
{
	if (UVRHandAnimInstance* Anim = Cast<UVRHandAnimInstance>(LeftHand->GetAnimInstance()))
	{
		Anim->SetPoseAlphaPoint(0);
	}
}

void AVRCharacter::PointRightTouchEnd()
{
	if (UVRHandAnimInstance* Anim = Cast<UVRHandAnimInstance>(RightHand->GetAnimInstance()))
	{
		Anim->SetPoseAlphaPoint(1);
	}
}

void AVRCharacter::PointLeftTouchEnd()
{
	if (UVRHandAnimInstance* Anim = Cast<UVRHandAnimInstance>(LeftHand->GetAnimInstance()))
	{
		Anim->SetPoseAlphaPoint(1);
	}
}

