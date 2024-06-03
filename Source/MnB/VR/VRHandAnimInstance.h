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
	void SetPoseAlphaGrap(float value) { PoseAlphaGrap = value; }
	void SetPoseAlphaPoint(float value) { PoseAlphaPoint = value; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bMirror = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PoseAlphaGrap = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PoseAlphaPoint = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentPoseAlphaPoint = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PoseAlphaIndexCurl = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PoseAlphaThumb = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentPoseAlphaThumb = 1.f;
};
