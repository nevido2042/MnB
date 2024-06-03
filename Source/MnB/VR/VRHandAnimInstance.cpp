// Fill out your copyright notice in the Description page of Project Settings.


#include "VRHandAnimInstance.h"

#include "VRHandSkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "VR/VRHandSkeletalMeshComponent.h"
#if WITH_EDITOR
#include "Animation/DebugSkelMeshComponent.h"
#include "Animation/SkeletalMeshActor.h"
#endif

void UVRHandAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	USkeletalMeshComponent* SkeletalMeshComponent = GetOwningComponent();
#if WITH_EDITOR
	UDebugSkelMeshComponent* DebugSkelMeshComponent = Cast<UDebugSkelMeshComponent>(SkeletalMeshComponent);
	if (DebugSkelMeshComponent) { return; }

	AActor* Actor = GetOwningActor();
	ASkeletalMeshActor* SkeletalMeshActor = Cast<ASkeletalMeshActor>(Actor);
	if (SkeletalMeshActor) { return; }
#endif

	UVRHandSkeletalMeshComponent* VRHandSkeletalMeshComponent = Cast<UVRHandSkeletalMeshComponent>(SkeletalMeshComponent);
	check(VRHandSkeletalMeshComponent);
	bMirror = VRHandSkeletalMeshComponent->bMirror;

}

void UVRHandAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CurrentPoseAlphaPoint = UKismetMathLibrary::FInterpTo(CurrentPoseAlphaPoint, PoseAlphaPoint, DeltaSeconds, 13.0);
	CurrentPoseAlphaThumb = UKismetMathLibrary::FInterpTo(CurrentPoseAlphaThumb, PoseAlphaThumb, DeltaSeconds, 13.0);

}
