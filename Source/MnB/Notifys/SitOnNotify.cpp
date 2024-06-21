// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/SitOnNotify.h"
#include "MnBCharacter.h"
#include "Horse/Horse.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/MnBCharacterAnimInstance.h"
#include "AnimInstance/HorseAnimInstance.h"


void USitOnNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AMnBCharacter* Char = Cast<AMnBCharacter>(MeshComp->GetOwner()))
	{
		Char->GetCurHorse()->SetCapsuleCollisionProfileName(TEXT("Vehicle"));

		Char->GetCharacterMovement()->GravityScale = 0.f;

		AHorse* Horse = Char->GetCurHorse();

		//Horse->GetSitLoc()->setchild
		FVector SitLoc = Horse->GetSitLocation()->GetComponentLocation();
		FRotator SitRot = Horse->GetSitLocation()->GetComponentRotation();
		
		Char->SetActorLocation(SitLoc);
		Char->SetActorRotation(SitRot);
		Char->GetController()->SetControlRotation(SitRot);

		//Char->AttachToActor(Horse, FAttachmentTransformRules::KeepWorldTransform);

		//Char->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

		UMnBCharacterAnimInstance* Anim = Cast<UMnBCharacterAnimInstance>(Char->GetMesh()->GetAnimInstance());
		Anim->SetRide(true);

		UHorseAnimInstance* HorseAnim = Cast<UHorseAnimInstance>(Horse->GetMesh()->GetAnimInstance());
		HorseAnim->SetOwnerMovement(Char);

		//Char->bOnHorse = true;
	}
}
