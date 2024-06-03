// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "VRHandSkeletalMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UVRHandSkeletalMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMirror = false;
	
};
