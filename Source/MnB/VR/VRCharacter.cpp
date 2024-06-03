// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"

#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "VRHandSkeletalMeshComponent.h"
#include "VRHandAnimInstance.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "VRHandAnimInstance.h"
#include "InputActionValue.h"

namespace
{
	const FName LeftGrip = TEXT("LeftGrip");
	const FName RightGrip = TEXT("RightGrip");
}

UVRInputDataConfig::UVRInputDataConfig()
{
	{
		static ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
		{ TEXT("/Script/EnhancedInput.InputMappingContext'/Game/MyAssets/VR/Input/IMC_Hands.IMC_Hands'") };
		check(Asset.Object);
		InputMappingContext = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/MyAssets/VR/Input/IA_Grab_Left.IA_Grab_Left'") };
		check(Asset.Object);
		IA_Grab_Left = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/MyAssets/VR/Input/IA_Grab_Right.IA_Grab_Right'") };
		check(Asset.Object);
		IA_Grab_Right = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/MyAssets/VR/Input/IA_Point_Left.IA_Point_Left'") };
		check(Asset.Object);
		IA_Point_Left = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/MyAssets/VR/Input/IA_Point_Right.IA_Point_Right'") };
		check(Asset.Object);
		IA_Point_Right = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/MyAssets/VR/Input/IA_IndexCurl_Left.IA_IndexCurl_Left'") };
		check(Asset.Object);
		IA_IndexCurl_Left = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/MyAssets/VR/Input/IA_IndexCurl_Right.IA_IndexCurl_Right'") };
		check(Asset.Object);
		IA_IndexCurl_Right = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/MyAssets/VR/Input/IA_Thumb_Left.IA_Thumb_Left'") };
		check(Asset.Object);
		IA_Thumb_Left = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/MyAssets/VR/Input/IA_Thumb_Right.IA_Thumb_Right'") };
		check(Asset.Object);
		IA_Thumb_Right = Asset.Object;
	}
}

// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(RootComponent);

	MotionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerLeft"));
	MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerRight"));

	MotionControllerLeft->SetupAttachment(RootComponent);
	MotionControllerRight->SetupAttachment(RootComponent);

	MotionControllerLeft->SetTrackingMotionSource(LeftGrip);
	MotionControllerRight->SetTrackingMotionSource(RightGrip);

	HandGraphLeft = NewObject<UHandGraph>(this, TEXT("HandGraphLeft"));
	HandGraphRight = NewObject<UHandGraph>(this, TEXT("HandGraphRight"));

	HandGraphLeft->Init(MotionControllerLeft);
	HandGraphRight->Init(MotionControllerRight);

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
			const UVRInputDataConfig* VRInputDataConfig = GetDefault<UVRInputDataConfig>();
			Subsystem->AddMappingContext(VRInputDataConfig->InputMappingContext, 0);
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

		HandGraphLeft->SetupPlayerInputComponent(EnhancedInputComponent);
		HandGraphRight->SetupPlayerInputComponent(EnhancedInputComponent);
	}

}

void UHandGraph::SetupPlayerInputComponent(UEnhancedInputComponent* InputComponent)
{
	const UVRInputDataConfig* VRInputDataConfig = GetDefault<UVRInputDataConfig>();
	if (MotionControllerComponent->MotionSource == LeftGrip)
	{
		InputComponent->BindAction(VRInputDataConfig->IA_Grab_Left, ETriggerEvent::Triggered, this, &ThisClass::OnGrabTriggered);
	}
	else if (MotionControllerComponent->MotionSource == RightGrip)
	{

	}
	else { check(false); }
}

void UHandGraph::OnGrabTriggered(const FInputActionValue& InputActionValue)
{
}

