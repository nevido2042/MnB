// Copyright Epic Games, Inc. All Rights Reserved.

#include "MnBGameMode.h"
#include "MnBCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMnBGameMode::AMnBGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
