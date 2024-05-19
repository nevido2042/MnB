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
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> UserWidget;

	class UUserWidget* CurrentWidget;
};



