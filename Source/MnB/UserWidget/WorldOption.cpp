// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/WorldOption.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UWorldOption::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	EnterTheTown = Cast<UButton>(GetWidgetFromName(TEXT("UI_EnterTheTownButton")));

	EnterTheTown->OnClicked.AddDynamic(this,&ThisClass::LoadTownLevel);
}

void UWorldOption::LoadTownLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("LoadTownLevel"));
	
	/*FLatentActionInfo LatentInfo;
	UGameplayStatics::LoadStreamLevel(this, TEXT("Demo"), true, true, LatentInfo);*/
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Demo"));
}
