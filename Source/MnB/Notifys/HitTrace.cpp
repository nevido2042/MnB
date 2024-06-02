// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/HitTrace.h"
#include "Weapons/Weapon.h"
#include "MnBCharacter.h"

void UHitTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AActor* const Actor = MeshComp->GetOwner();
	AMnBCharacter* const MnBCharacter = Cast<AMnBCharacter>(Actor);
	if (MnBCharacter)
	{
		Weapon = MnBCharacter->GetEquippedWeapon();
	}
}

void UHitTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (Weapon == nullptr) { return; }

	Weapon->HitDitect();
}

void UHitTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Weapon = nullptr;
}
