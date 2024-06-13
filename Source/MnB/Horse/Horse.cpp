// Fill out your copyright notice in the Description page of Project Settings.


#include "Horse/Horse.h"

#include "Components/Health.h"
#include "Data/Input/BasicInputDataConfig.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AHorse::AHorse()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = CreateDefaultSubobject<UHealth>(TEXT("Health"));

	BodyCollsion = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCollision"));
	BodyCollsion->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AHorse::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			const UBasicInputDataConfig* BasicInputDataConfig = GetDefault<UBasicInputDataConfig>();
			Subsystem->AddMappingContext(BasicInputDataConfig->InputMappingContext, 0);

			//const UVRHandsInputDataConfig* VRHandsInputDataConfig = GetDefault<UVRHandsInputDataConfig>();
			//Subsystem->AddMappingContext(VRHandsInputDataConfig->InputMappingContext, 0);

			//// HandGraphLeft, HandGraphRight
			//const UVRHandsAnimationInputDataConfig* VRHandsAnimationInputDataConfig = GetDefault<UVRHandsAnimationInputDataConfig>();
			//Subsystem->AddMappingContext(VRHandsAnimationInputDataConfig->InputMappingContext, 1);
		}
	}
}

// Called every frame
void AHorse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHorse::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		{
			const UBasicInputDataConfig* BasicInputDataConfig = GetDefault<UBasicInputDataConfig>();
			EnhancedInputComponent->BindAction(BasicInputDataConfig->Move, ETriggerEvent::Triggered, this, &ThisClass::OnMove);
		}
	}

}

#include "VR/VRCharacter.h"
void AHorse::Interact(AActor* InActor)
{
	if (AVRCharacter* VRCharacter = Cast<AVRCharacter>(InActor))
	{
		VRCharacter->StartSitOnHorse();
	}

}

float AHorse::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health->AddCurrentHP(-Damage);

	if (FMath::IsNearlyZero(Health->GetCurrentHP()))
	{
		Destroy();
	}

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void AHorse::OnMove(/*const FInputActionValue& InputActionValue*/)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, TEXT("Horse"));

	/*const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

	if (!FMath::IsNearlyZero(ActionValue.Y))
	{
		const FVector ForwardVector = GetActorForwardVector();
		AddMovementInput(ForwardVector, ActionValue.Y);
	}

	AddControllerYawInput(ActionValue.X);*/
}

