// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/DescendingHorseNotify.h"

#include "MnBCharacter.h"
#include "Horse/Horse.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/MnBCharacterAnimInstance.h"

void UDescendingHorseNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AMnBCharacter* Char = Cast<AMnBCharacter>(MeshComp->GetOwner()))
	{
		AHorse* Horse = Char->GetCurHorse();
		Char->GetCharacterMovement()->GravityScale = 1.f;
		Horse->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		
		FVector Loc = Horse->GetLeft()->GetComponentLocation();
		Char->SetActorLocation(Loc);

		UMnBCharacterAnimInstance* Anim = Cast<UMnBCharacterAnimInstance>(Char->GetMesh()->GetAnimInstance());
		Anim->SetRide(false);

		Horse->SetCapsuleCollisionProfileName(TEXT("Horse"));
	}
}
