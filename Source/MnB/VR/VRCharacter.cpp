// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "VRHandSkeletalMeshComponent.h"

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

		/*{
			"Tagged": [
				[
					"RelativeLocation",
						"(X=-2.981260,Y=3.500000,Z=4.561753)"
				],
					[
						"RelativeRotation",
							"(Pitch=25.000000,Yaw=0.000000,Roll=89.999999)"
					],
					[
						"RelativeScale3D",
							"(X=1.000000,Y=1.000000,Z=1.000000)"
					]
			]
		}*/

	}
	{
		ConstructorHelpers::FObjectFinder<USkeletalMesh>Finder(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
		LeftHand->SetSkeletalMesh(Finder.Object);

		const FTransform Transform = FTransform(FRotator(-25.000000, -179.999999, 89.999998), FVector(-2.981260, -3.500000, 4.561753));
		LeftHand->SetRelativeTransform(Transform);

		/*{
			"Tagged": [
				[
					"RelativeLocation",
						"(X=-2.981260,Y=-3.500000,Z=4.561753)"
				],
					[
						"RelativeRotation",
							"(Pitch=-25.000000,Yaw=-179.999999,Roll=89.999998)"
					],
					[
						"RelativeScale3D",
							"(X=1.000000,Y=1.000000,Z=1.000000)"
					]
			]
		}*/

		LeftHand->bMirror = true;
	}


}	 

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

}

