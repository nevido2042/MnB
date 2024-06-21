// Copyright Epic Games, Inc. All Rights Reserved.

#include "MnBGameMode.h"
#include "MnBCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "GameFramework/GameUserSettings.h"

AMnBGameMode::AMnBGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}

	{
		static ConstructorHelpers::FClassFinder<APawn>Finder(TEXT("/Script/Engine.Blueprint'/Game/MyAssets/VR/BP_VRCharacter.BP_VRCharacter_C'"));
		if (Finder.Class)
		{
			VRPawnClass = Finder.Class;
		}

	}

	{
		static ConstructorHelpers::FClassFinder<APlayerController>Finder(TEXT("/Script/Engine.Blueprint'/Game/MyAssets/VR/BPC_VR.BPC_VR_C'"));
		if (Finder.Class)
		{
			VRPlayerControllerClass = Finder.Class;
		}

	}

	{
		static ConstructorHelpers::FClassFinder<APawn>Finder(TEXT("/Script/Engine.Blueprint'/Game/MyAssets/Character/BP_MnBCharacter.BP_MnBCharacter_C'"));
		if (Finder.Class)
		{
			PCPawnClass = Finder.Class;
		}
	}

	{
		static ConstructorHelpers::FClassFinder<APlayerController>Finder(TEXT("/Script/Engine.Blueprint'/Game/MyAssets/Controller/BPC_PC.BPC_PC_C'"));
		if (Finder.Class)
		{
			PCPlayerControllerClass = Finder.Class;
		}

	}
}

void AMnBGameMode::BeginPlay()
{
	Super::BeginPlay();
}

APlayerController* AMnBGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	UGameUserSettings* GameUserSetting = UGameUserSettings::GetGameUserSettings();

	const bool bVR = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
	if (bVR)
	{
		DefaultPawnClass = VRPawnClass;
		PlayerControllerClass = VRPlayerControllerClass;

		GameUserSetting->SetViewDistanceQuality(0);
		GameUserSetting->SetAntiAliasingQuality(0);
		GameUserSetting->SetPostProcessingQuality(0);
		GameUserSetting->SetShadowQuality(0);
		GameUserSetting->SetGlobalIlluminationQuality(0);
		GameUserSetting->SetReflectionQuality(0);
		GameUserSetting->SetTextureQuality(0);
		GameUserSetting->SetVisualEffectQuality(0);
		GameUserSetting->SetFoliageQuality(0);
		GameUserSetting->SetShadingQuality(0);

		GameUserSetting->ApplySettings(false);
	}
	else
	{
		DefaultPawnClass = PCPawnClass;
		PlayerControllerClass = PCPlayerControllerClass;

		GameUserSetting->SetViewDistanceQuality(2);
		GameUserSetting->SetAntiAliasingQuality(2);
		GameUserSetting->SetPostProcessingQuality(2);
		GameUserSetting->SetShadowQuality(2);
		GameUserSetting->SetGlobalIlluminationQuality(2);
		GameUserSetting->SetReflectionQuality(2);
		GameUserSetting->SetTextureQuality(2);
		GameUserSetting->SetVisualEffectQuality(2);
		GameUserSetting->SetFoliageQuality(2);
		GameUserSetting->SetShadingQuality(2);

		GameUserSetting->ApplySettings(false);
	}
	
	return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
}

//void AMnBGameMode::SetStartInVR(bool bValue)
//{
//
//	// Get the project settings
//	UGeneralProjectSettings* ProjectSettings = GetMutableDefault<UGeneralProjectSettings>();
//	if (ProjectSettings)
//	{
//		// Change the value
//		ProjectSettings->bStartInVR = bValue;
//		// Save the settings
//		ProjectSettings->SaveConfig(CPF_Config, TEXT("DefaultGame.ini"));
//	}
//
//}
