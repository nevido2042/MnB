// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MnBCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UMnBCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	APawn* Pawn;

	class UPawnMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadOnly)
	bool IsShouldMove;

	UPROPERTY(BlueprintReadOnly)
	float Speed;

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
