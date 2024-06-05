// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MotionControllerComponent.h"
#include "VRHandAnimInstance.h"
#include "VR/HandGraph.h"
#include "VRCharacter.generated.h"

static inline const FName LeftGrip = TEXT("LeftGrip");
static inline const FName RightGrip = TEXT("RightGrip");

UCLASS()
class MNB_API AVRCharacter : public ACharacter
{
	friend class UHandGraph;
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void OnMove(const FInputActionValue& InputActionValue);

	void OnGrabLeftStarted(const FInputActionValue& InputActionValue) { OnGrabStarted(MotionControllerLeft, true, InputActionValue); }
	void OnGrabRightStarted(const FInputActionValue& InputActionValue) { OnGrabStarted(MotionControllerRight,false, InputActionValue); }
	void OnGrabStarted(UMotionControllerComponent* MotionControllerComponent, const bool bLeft, const FInputActionValue& InputActionValue);

	void OnGrabLeftCompleted(const FInputActionValue& InputActionValue) { OnGrabCompleted(MotionControllerLeft, true, InputActionValue); }
	void OnGrabRightCompleted(const FInputActionValue& InputActionValue) { OnGrabCompleted(MotionControllerRight, false, InputActionValue); }
	void OnGrabCompleted(UMotionControllerComponent* MotionControllerComponent, const bool bLeft, const FInputActionValue& InputActionValue);

private:
	class UCameraComponent* VRCamera = nullptr;

	class UMotionControllerComponent* MotionControllerLeft = nullptr;
	class UMotionControllerComponent* MotionControllerRight = nullptr;

	class UVRHandSkeletalMeshComponent* LeftHand = nullptr;
	class UVRHandSkeletalMeshComponent* RightHand = nullptr;

	UHandGraph* HandGraphLeft;
	UHandGraph* HandGraphRight;

private:
	AActor* LeftFocusingActor = nullptr;
	AActor* RightFocusingActor = nullptr;

	class AWeapon* LeftEquippedWeapon = nullptr;
	class AWeapon* RightEquippedWeapon = nullptr;

	void HandRayCast(UVRHandSkeletalMeshComponent * Hand);

	//UFUNCTION()
	void UnEquip(bool bLeft);

	void Interact(AActor* HandFoucsing);

public:
	void Equip(AActor* HandFoucsing);
};
