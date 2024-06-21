// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/SitOnNotify.h"
#include "MnBCharacter.h"
#include "Horse/Horse.h"
void USitOnNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (AMnBCharacter* Char = Cast<AMnBCharacter>(MeshComp->GetOwner()))
	{
		FVector SitLoc = Char->GetCurHorse()->GetSitLocation()->GetComponentLocation();
		Char->SetActorLocation(SitLoc);
	}
}

void USitOnNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
}

void USitOnNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
}
