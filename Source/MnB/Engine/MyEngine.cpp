// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/MyEngine.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "GeneralProjectSettings.h"

UMyEngine::UMyEngine()
{
}

bool UMyEngine::InitializeHMDDevice()
{
	const bool bVR = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected(); //UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
	if (bVR)
	{
		SetStartInVR(true);
	}
	else
	{

		SetStartInVR(false);
	}

	return Super::InitializeHMDDevice();
}

void UMyEngine::SetStartInVR(bool bValue)
{
	// Get the project settings
	UGeneralProjectSettings* ProjectSettings = GetMutableDefault<UGeneralProjectSettings>();
	if (ProjectSettings)
	{
		// Change the value
		ProjectSettings->bStartInVR = bValue;
		// Save the settings
		ProjectSettings->SaveConfig(CPF_Config, TEXT("DefaultGame.ini"));
	}
}
