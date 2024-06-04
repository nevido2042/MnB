// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "VRHandSkeletalMeshComponent.h"
#include "VRHandAnimInstance.h"
#include "InputMappingContext.h"
#include "InputAction.h"

#include "HandGraph.generated.h"

UCLASS()
class MNB_API UVRHandsAnimationInputDataConfig : public UObject
{
	GENERATED_BODY()

public:
	UVRHandsAnimationInputDataConfig();

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
class MNB_API UVRHandsInputDataConfig : public UObject
{
	GENERATED_BODY()

public:
	UVRHandsInputDataConfig();

	UInputMappingContext* InputMappingContext = nullptr;

public:
	UInputAction* IA_Grab_Left = nullptr;
	UInputAction* IA_Grab_Right = nullptr;

	/*UInputAction* IA_Point_Left = nullptr;
	UInputAction* IA_Point_Right = nullptr;

	UInputAction* IA_IndexCurl_Left = nullptr;
	UInputAction* IA_IndexCurl_Right = nullptr;

	UInputAction* IA_Thumb_Left = nullptr;
	UInputAction* IA_Thumb_Right = nullptr;*/
};

UCLASS()
class UHandGraph : public UObject
{
	GENERATED_BODY()

	enum EHandType
	{
		Left,
		Right
	};

public:
	void SetupPlayerInputComponent(UMotionControllerComponent* InMotionControllerComponent, UEnhancedInputComponent* InputComponent);
	void OnGrabTriggered(const FInputActionValue& InputActionValue);
	void OnGrabCompleted(const FInputActionValue& InputActionValue);

	void OnPointStarted(const FInputActionValue& InputActionValue);
	void OnPointCompleted(const FInputActionValue& InputActionValue);

	void OnIndexCurlTriggered(const FInputActionValue& InputActionValue);
	void OnIndexCurlCompleted(const FInputActionValue& InputActionValue);

	void OnThumbStarted(const FInputActionValue& InputActionValue);
	void OnThumbCompleted(const FInputActionValue& InputActionValue);

	UMotionControllerComponent* MotionControllerComponent = nullptr;
	UVRHandAnimInstance* VRHandAnimInstance = nullptr;
	EHandType HandType = (EHandType)-1;
};