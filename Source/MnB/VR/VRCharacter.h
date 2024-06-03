// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VRCharacter.generated.h"

UCLASS()
class MNB_API AVRCharacter : public ACharacter
{
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

private:
	class UCameraComponent* CameraComponent = nullptr;

	class UMotionControllerComponent* MotionControllerComponentLeft = nullptr;
	class UMotionControllerComponent* MotionControllerComponentRight = nullptr;

	class UVRHandSkeletalMeshComponent* LeftHand = nullptr;
	class UVRHandSkeletalMeshComponent* RightHand = nullptr;

private:
	class UInputMappingContext* MappingContext;

	class UInputAction* IA_GrabRight = nullptr;
	class UInputAction* IA_GrabLeft = nullptr;

	class UInputAction* IA_PointRight = nullptr;
	class UInputAction* IA_PointLeft = nullptr;

	void GrabRight(const struct FInputActionValue& Value);
	void GrabLeft(const struct FInputActionValue& Value);

	void PointRightTouch();
	void PointLeftTouch();
	void PointRightTouchEnd();
	void PointLeftTouchEnd();


};
