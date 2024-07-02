// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "AI/AICharacter.h"
#include "MnBCharacter.h"
#include "UserWidget/BattleSituationWidget.h"

void ABattleGameMode::BeginPlay()
{
	BattleSituationWidget = CreateWidget(GetWorld(), BattleSituationWidgetAsset);
	BattleSituationWidget->AddToViewport();

	UBattleSituationWidget* BattleSituation = Cast<UBattleSituationWidget>(BattleSituationWidget);
	BattleSituation->SetATeamBarPercent(1.f);
	BattleSituation->SetBTeamBarPercent(1.f);

	CountTeamUnits();
}

void ABattleGameMode::CountTeamUnits()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), OutActors);

	for (AActor* Actor : OutActors)
	{
		ETeam Team;

		if (AAICharacter* AI = Cast<AAICharacter>(Actor))
		{
			Team = AI->GetTeam();
		}
		else if (AMnBCharacter* PC = Cast<AMnBCharacter>(Actor))
		{
			Team = PC->GetTeam();
		}
		else
		{
			continue;
		}


		if (Team == ETeam::ATeam)
		{
			++ATeamInit;
		}
		else if(Team == ETeam::BTeam)
		{
			++BTeamInit;
		}
	}

	ATeamCur = ATeamInit;
	BTeamCur = BTeamInit;
}

void ABattleGameMode::Decrease(ETeam team)
{
	//BattleSituation->SetBTeamBarPercent(BTeamCur / BTeamInit);
}
