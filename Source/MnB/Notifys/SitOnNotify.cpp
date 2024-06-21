// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/SitOnNotify.h"
#include "MnBCharacter.h"
#include "Horse/Horse.h"
#include "GameFramework/CharacterMovementComponent.h"

//void USitOnNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
//{
//	if (AMnBCharacter* Char = Cast<AMnBCharacter>(MeshComp->GetOwner()))
//	{
//		FVector SitLoc = Char->GetCurHorse()->GetSitLocation()->GetComponentLocation();
//		Char->SetActorLocation(SitLoc);
//	}
//}
//
//void USitOnNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
//{
//}
//
//void USitOnNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
//{
//}

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

		Horse->AttachToActor(Char, FAttachmentTransformRules::KeepWorldTransform);
		//Horse->AddActorLocalRotation(FRotator(0.f, -90.f, 0.f));
	}
}
