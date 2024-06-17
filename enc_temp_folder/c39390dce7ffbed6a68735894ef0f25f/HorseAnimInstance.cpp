// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/HorseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

void UHorseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Pawn = TryGetPawnOwner();
	if (!Pawn) { return; }
}

void UHorseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Pawn) { return; }

	if (Pawn->GetParentActor() == nullptr) return;

	if (ACharacter* Char = Cast<ACharacter>(Pawn->GetParentActor()))
	{
		OwnerMovement = Char->GetCharacterMovement();
	}

	if (OwnerMovement)
	{
		Speed = OwnerMovement->Velocity.Length();
	}

}
