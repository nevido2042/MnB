// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/HitTrace.h"
#include "Weapons/Weapon.h"
#include "MnBCharacter.h"
#include "AI/AICharacter.h"

void UHitTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AWeapon* Weapon = GetWeaponInfo(MeshComp);
	if (Weapon == nullptr) { return; }
}

void UHitTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	AWeapon* Weapon = GetWeaponInfo(MeshComp);
	if (Weapon == nullptr) { return; }
	Weapon->HitDitect();
}

void UHitTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AWeapon* Weapon = GetWeaponInfo(MeshComp);
	if (Weapon == nullptr) { return; }

	Weapon->SetIsApplyDamage(false);

	AActor* const Actor = MeshComp->GetOwner();
	AMnBCharacter* const MnBCharacter = Cast<AMnBCharacter>(Actor);
	if (MnBCharacter)
	{
		MnBCharacter->SetCurrentAttackDirection(EAttackDirection::AttackNone);
	}
}

AWeapon* UHitTrace::GetWeaponInfo(USkeletalMeshComponent* MeshComp)
{
	AWeapon* Weapon = nullptr;

	AActor* const Actor = MeshComp->GetOwner();

	AMnBCharacter* const MnBCharacter = Cast<AMnBCharacter>(Actor);
	if (MnBCharacter)
	{
		Weapon = MnBCharacter->GetEquippedWeapon();
	}

	AAICharacter* const AICharacter = Cast<AAICharacter>(Actor);
	if (AICharacter)
	{
		Weapon = AICharacter->GetCurrentWeapon();
	}
	return Weapon;
}
