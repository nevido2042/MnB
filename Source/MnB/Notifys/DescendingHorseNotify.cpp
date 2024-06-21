// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/DescendingHorseNotify.h"

#include "MnBCharacter.h"
#include "Horse/Horse.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/MnBCharacterAnimInstance.h"
#include "AnimInstance/HorseAnimInstance.h"

void UDescendingHorseNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AMnBCharacter* Char = Cast<AMnBCharacter>(MeshComp->GetOwner()))
	{
		AHorse* Horse = Char->GetCurHorse();
		Char->GetCharacterMovement()->GravityScale = 1.f;
		Char->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		
		FVector Loc = Horse->GetLeft()->GetComponentLocation();
		Char->SetActorLocation(Loc);

		UMnBCharacterAnimInstance* Anim = Cast<UMnBCharacterAnimInstance>(Char->GetMesh()->GetAnimInstance());
		Anim->SetRide(false);

		Horse->SetCapsuleCollisionProfileName(TEXT("Pawn"));

		UHorseAnimInstance* HorseAnim = Cast<UHorseAnimInstance>(Horse->GetMesh()->GetAnimInstance());
		HorseAnim->SetOwnerMovement(nullptr);

		Char->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

		Char->SetCurHorse(nullptr);
		//Char->bOnHorse = false;
	}
}
