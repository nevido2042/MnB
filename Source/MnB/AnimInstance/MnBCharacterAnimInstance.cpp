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

	Character = Cast<ACharacter>(Pawn);

	MnBCharacter = Cast<AMnBCharacter>(Character);
}

void UMnBCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Pawn) { return; }

	Speed = MovementComponent->Velocity.Length();

	//IsShouldMove
	if (Speed > 3.0f)
	{
		bShouldMove = true;
	}
	else
	{
		bShouldMove = false;
	}

	//bRedayToRightAttack
	if (MnBCharacter)
	{
		bReadyToRightAttack = MnBCharacter->IsReadyToRightAttack();
		bReadyToLeftAttack = MnBCharacter->IsReadyToLeftAttack();
	}
}
