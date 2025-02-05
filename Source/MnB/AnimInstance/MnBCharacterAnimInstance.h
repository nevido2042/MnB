// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MnBCharacter.h"
#include "Enum/Enums.h"
#include "MnBCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UMnBCharacterAnimInstance : public UAnimInstance
{
	friend AMnBCharacter;

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

	UPROPERTY(BlueprintReadOnly)
	bool bRide = false;


	UPROPERTY(BlueprintReadOnly)
	EGuardDirection GuardDir = EGuardDirection::GuardNone;

	UPROPERTY(BlueprintReadOnly)
	EBlockedDirection BlockedDir = EBlockedDirection::None;

	UPROPERTY(BlueprintReadOnly)
	bool bBow = false;

	UPROPERTY(BlueprintReadOnly)
	bool bBowDraw = false;

	UPROPERTY(BlueprintReadOnly)
	float ControlYaw;

	UPROPERTY(BlueprintReadOnly)
	float ControlPitch;

	UPROPERTY(BlueprintReadOnly)
	FVector CharacterForwardVector;

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	void IsShouldMove();
	void GetMoveDirection();
	void IsReadyToAttack();

public:
	void SetBlockedDirection(EBlockedDirection Value) { BlockedDir = Value; }

	void SetGuardDirection(EGuardDirection Value) { GuardDir = Value; }

	void SetRide(bool Value) { bRide = Value; }

	void SetBowDraw(bool Value) { bBowDraw = Value; }

	bool GetBowDraw() { return bBowDraw; }

	void SetBow(bool Value) { bBow = Value; }

	void SetControlYawPitch();

	void SetCharacterForwardVector();
};
