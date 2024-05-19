// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/MnBCharacterAnimInstance.h"
#include "MnBCharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

void UMnBCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Pawn = TryGetPawnOwner();
	if (!Pawn) { return; }

	MovementComponent = Pawn->GetMovementComponent();
}

void UMnBCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Pawn) { return; }

	Speed = MovementComponent->Velocity.Length();

	//IsShouldMove//
	FVector Acceleration;
	ACharacter * Character = Cast<ACharacter>(Pawn);
	if (Character)
	{
		Acceleration = Character->GetCharacterMovement()->GetCurrentAcceleration();
	}

	FVector ZeroVector = FVector::ZeroVector;

	if (Acceleration != ZeroVector && Speed > 3.0f)
	{
		IsShouldMove = true;
	}
	else
	{
		IsShouldMove = false;
	}
}
