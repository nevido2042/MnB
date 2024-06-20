// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MnBCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UENUM(BlueprintType)
enum EBlockedDirection
{
	Left = -1,
	None = 0,
	Right = 1
};

UENUM(BlueprintType)
enum EAttackDirection
{
	AttackLeft = -1,
	AttackNone = 0,
	AttackRight = 1
};

UENUM(BlueprintType)
enum EGuardDirection
{
	GuardLeft = -1,
	GuardNone = 0,
	GarudRight = 1
};

UCLASS(config=Game)
class AMnBCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Guard Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GuardAction;

public:
	AMnBCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	bool IsReadyToRightAttack() { return bReadyToRightAttack; }
	bool IsReadyToLeftAttack() { return bReadyToLeftAttack; }
	FVector2D GetLookAxisVector(){ return LookAxisVector; }
	class AWeapon* GetEquippedWeapon() { return EquippedWeapon; }
	void Equip();

protected:
	bool bReadyToAttack = false;
	bool bReadyToRightAttack = false;
	bool bReadyToLeftAttack = false;

	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackRightAnimMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackLeftAnimMontage;

	FVector2D LookAxisVector;

	AActor* FocusingActor = nullptr;

	class AWeapon* EquippedWeapon = nullptr;

	class AShield* EquippedShield = nullptr;

	EAttackDirection CurAttackDir = EAttackDirection::AttackNone;

	class UHealth* Health = nullptr;

	UAnimMontage* DamagedMontage = nullptr;

protected:
	void ReadyToAttack();
	void Attack();
	void CameraRaycast();
	void Interact();
	void RecoverBlockedState();
	void Guard();
	void GuardEnd();

public:
	void Blocked();
	void SetCurrentAttackDirection(EAttackDirection value) { CurAttackDir = value; }
private:
	class UAIPerceptionStimuliSourceComponent* StimulusSource;

	void SetupStimulusSource();

private:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	void UpdateActorInfo(); 

protected:
	UPROPERTY(EditAnywhere)
	class UAudioComponent * Audio = nullptr;

	UPROPERTY(EditAnywhere)

	TArray<class USoundBase*> HitSounds;

private:
	void SetRandomSoundAndPlay();

};

