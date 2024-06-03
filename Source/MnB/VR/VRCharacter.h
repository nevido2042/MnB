// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "MotionControllerComponent.h"
#include "VRCharacter.generated.h"

UCLASS()
class MNB_API UVRInputDataConfig : public UObject
{
	GENERATED_BODY()

public:
	UVRInputDataConfig();

	UInputMappingContext* InputMappingContext = nullptr;

public:
	UInputAction* IA_Grab_Left = nullptr;
	UInputAction* IA_Grab_Right = nullptr;

	UInputAction* IA_Point_Left = nullptr;
	UInputAction* IA_Point_Right = nullptr;

	UInputAction* IA_IndexCurl_Left = nullptr;
	UInputAction* IA_IndexCurl_Right = nullptr;

	UInputAction* IA_Thumb_Left = nullptr;
	UInputAction* IA_Thumb_Right = nullptr;
};

UCLASS()
class UHandGraph : public UObject
{
	GENERATED_BODY()

public:
	void Init(UMotionControllerComponent* InMotionControllerComponent)
	{
		MotionControllerComponent = InMotionControllerComponent;
	}
	void SetupPlayerInputComponent(UEnhancedInputComponent* InputComponent);
	void OnGrabTriggered(const FInputActionValue& InputActionValue);

	UMotionControllerComponent* MotionControllerComponent;
};

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

	class UMotionControllerComponent* MotionControllerLeft = nullptr;
	class UMotionControllerComponent* MotionControllerRight = nullptr;

	class UVRHandSkeletalMeshComponent* LeftHand = nullptr;
	class UVRHandSkeletalMeshComponent* RightHand = nullptr;

	UHandGraph* HandGraphLeft;
	UHandGraph* HandGraphRight;
};
