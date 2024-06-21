// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HorseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UHorseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	APawn* Pawn;

	class UCharacterMovementComponent* OwnerMovement = nullptr;

	UPROPERTY(BlueprintReadOnly)
	float Speed;
};
