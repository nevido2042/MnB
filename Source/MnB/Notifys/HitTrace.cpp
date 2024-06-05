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

	bHit = false;
}

void UHitTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (Weapon == nullptr) { return; }

	if (bHit == true) { return; } // 한 번만 데미지 줌

	bHit = Weapon->HitDitect();

	if (bHit == false)
	{
		Weapon->ObstacleDitect();
	}
}

void UHitTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Weapon = nullptr;

	AActor* const Actor = MeshComp->GetOwner();
	AMnBCharacter* const MnBCharacter = Cast<AMnBCharacter>(Actor);
	if (MnBCharacter)
	{
		MnBCharacter->SetCurrentAttackDirection(EAttackDirection::AttackNone);
	}
}
