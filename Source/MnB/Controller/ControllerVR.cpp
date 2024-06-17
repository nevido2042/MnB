// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/ControllerVR.h"
#include "Data/Input/BasicInputDataConfig.h"

void AControllerVR::SetupInputComponent()
{

	const UBasicInputDataConfig* BasicInputDataConfig = GetDefault<UBasicInputDataConfig>();
	/*InputComponent->BindAction()
	InputComponent->BindAction(BasicInputDataConfig->Move, ETriggerEvent::Triggered, this, &ThisClass::OnMove);
	InputComponent->BindAction(BasicInputDataConfig->Look, ETriggerEvent::Triggered, this, &ThisClass::OnLook);*/
}
