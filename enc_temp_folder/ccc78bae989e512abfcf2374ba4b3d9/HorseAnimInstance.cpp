// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/HorseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "VR/VRCharacter.h"
#include "MnBCharacter.h"
#include "Horse/Horse.h"

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

	if (AVRCharacter* Char = Cast<AVRCharacter>(Pawn->GetParentActor()))
	{
		OwnerMovement = Char->GetCharacterMovement();
	}

	if (AHorse* Horse = Cast<AHorse>(Pawn))
	{
		if (Horse->GetRider())
		{
			OwnerMovement = Horse->GetRider()->GetCharacterMovement();
		}
	}

	if (OwnerMovement)
	{
		Speed = OwnerMovement->Velocity.Length();
	}

}
