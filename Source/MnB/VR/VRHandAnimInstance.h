// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VRHandAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UVRHandAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMirror = false;
};
