// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/FightOption.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UFightOption::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AttackBtn = Cast<UButton>(GetWidgetFromName(TEXT("UI_AttackBtn")));

	AttackBtn->OnClicked.AddDynamic(this, &ThisClass::LoadFightLevel);
}

void UFightOption::LoadFightLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("Load Fight Level"));
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("FightLevel"));
}
