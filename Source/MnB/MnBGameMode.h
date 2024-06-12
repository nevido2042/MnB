// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MnBGameMode.generated.h"

UCLASS(minimalapi)
class AMnBGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMnBGameMode();

	virtual void BeginPlay();

protected:
	TSubclassOf<APawn> VRPawnClass = nullptr;
	TSubclassOf<APlayerController> VRPlayerControllerClass = nullptr;

	TSubclassOf<APawn> PCPawnClass = nullptr;
	TSubclassOf<APlayerController> PCPlayerControllerClass = nullptr;

	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	/*bool bVRMode = false;

	void SetStartInVR(bool bValue);

	class UMyEngine MyEngine;*/
};



