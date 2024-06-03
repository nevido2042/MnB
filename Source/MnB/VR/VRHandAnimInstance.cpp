// Fill out your copyright notice in the Description page of Project Settings.


#include "VRHandAnimInstance.h"
#include "VRHandSkeletalMeshComponent.h"

void UVRHandAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	const UVRHandSkeletalMeshComponent* VRHandSkeletalMeshComponent = Cast<UVRHandSkeletalMeshComponent>(GetOwningComponent());
	if (VRHandSkeletalMeshComponent)
	{
		bMirror = VRHandSkeletalMeshComponent->bMirror;
	}

}

void UVRHandAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}
