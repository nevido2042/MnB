// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/MnBCharacterAnimInstance.h"
#include "MnBCharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "MnBCharacter.h"

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

	//IsShouldMove
	FVector Acceleration;
	ACharacter * Character = Cast<ACharacter>(Pawn);
	if (Character)
	{
		Acceleration = Character->GetCharacterMovement()->GetCurrentAcceleration();
	}

	FVector ZeroVector = FVector::ZeroVector;

	if (Acceleration != ZeroVector && Speed > 3.0f)
	{
		bShouldMove = true;
	}
	else
	{
		bShouldMove = false;
	}

	//bRedayToRightAttack
	MnBCharacter = Cast<AMnBCharacter>(Character);
	if (MnBCharacter)
	{
		bReadyToRightAttack = MnBCharacter->IsReadyToRightAttack();
		bReadyToRightAttack = MnBCharacter->IsReadyToLeftAttack();
	}
}
