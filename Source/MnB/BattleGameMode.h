// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MnBGameMode.h"
#include "BattleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API ABattleGameMode : public AMnBGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> BattleSituationWidgetAsset;

	class UUserWidget* BattleSituationWidget = nullptr;

	uint8 ATeamInit = 0;
	uint8 BTeamInit = 0;
	uint8 ATeamCur = 0;
	uint8 BTeamCur = 0;

private:
	void CountTeamUnits();

public:
	void Decrease(ETeam team);
	
};
