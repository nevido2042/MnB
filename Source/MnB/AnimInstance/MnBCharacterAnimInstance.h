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

	class AMnBCharacter* MnBCharacter;

	ACharacter* Character;

	UPROPERTY(BlueprintReadOnly)
	bool bShouldMove;

	UPROPERTY(BlueprintReadOnly)
	float Speed;

	UPROPERTY(BlueprintReadOnly)
	bool bReadyToRightAttack;

	UPROPERTY(BlueprintReadOnly)
	bool bReadyToLeftAttack;

	UPROPERTY(BlueprintReadOnly)
	float MoveDirection;

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	void IsShouldMove();
	void GetMoveDirection();
	void IsReadyToAttack();
};
