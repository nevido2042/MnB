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
	void OnGrabLeftStarted(const FInputActionValue& InputActionValue) { OnGrabStarted(MotionControllerLeft, InputActionValue); }
	void OnGrabRightStarted(const FInputActionValue& InputActionValue) { OnGrabStarted(MotionControllerRight, InputActionValue); }
	void OnGrabStarted(UMotionControllerComponent* MotionControllerComponent, const FInputActionValue& InputActionValue);

	void OnGrabLeftCompleted(const FInputActionValue& InputActionValue) { OnGrabCompleted(MotionControllerLeft, InputActionValue); }
	void OnGrabRightCompleted(const FInputActionValue& InputActionValue) { OnGrabCompleted(MotionControllerRight, InputActionValue); }
	void OnGrabCompleted(UMotionControllerComponent* MotionControllerComponent, const FInputActionValue& InputActionValue);

private:
	class UCameraComponent* CameraComponent = nullptr;

	class UMotionControllerComponent* MotionControllerLeft = nullptr;
	class UMotionControllerComponent* MotionControllerRight = nullptr;

	class UVRHandSkeletalMeshComponent* LeftHand = nullptr;
	class UVRHandSkeletalMeshComponent* RightHand = nullptr;

	UHandGraph* HandGraphLeft;
	UHandGraph* HandGraphRight;
};
