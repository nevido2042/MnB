// Copyright Epic Games, Inc. All Rights Reserved.

#include "MnBGameMode.h"
#include "MnBCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "HeadMountedDisplayFunctionLibrary.h"

AMnBGameMode::AMnBGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	{
		ConstructorHelpers::FClassFinder<APawn>Finder(TEXT("/Script/Engine.Blueprint'/Game/MyAssets/VR/BP_VRCharacter.BP_VRCharacter_C'"));
		if (Finder.Class)
		{
			VRPawnClass = Finder.Class;
		}

	}

	{
		ConstructorHelpers::FClassFinder<APlayerController>Finder(TEXT("/Script/Engine.Blueprint'/Game/MyAssets/VR/BPC_VR.BPC_VR_C'"));
		if (Finder.Class)
		{
			VRPlayerControllerClass = Finder.Class;
		}

	}

	{
		ConstructorHelpers::FClassFinder<APawn>Finder(TEXT("/Script/Engine.Blueprint'/Game/MyAssets/Character/BP_MnBCharacter.BP_MnBCharacter_C'"));
		if (Finder.Class)
		{
			PCPawnClass = Finder.Class;
		}
	}

	{
		ConstructorHelpers::FClassFinder<APlayerController>Finder(TEXT("/Script/Engine.Blueprint'/Game/MyAssets/Controller/BPC_PC.BPC_PC_C'"));
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
	const bool bVR = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
	if (bVR)
	{
		DefaultPawnClass = VRPawnClass;
		PlayerControllerClass = VRPlayerControllerClass;
	}
	else
	{
		DefaultPawnClass = PCPawnClass;
		PlayerControllerClass = PCPlayerControllerClass;
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
