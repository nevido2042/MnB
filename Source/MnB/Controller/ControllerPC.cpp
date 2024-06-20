// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/ControllerPC.h"
#include "Blueprint/UserWidget.h"

AControllerPC::AControllerPC()
{
	{
		static ConstructorHelpers::FClassFinder<UUserWidget>Finder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyAssets/UI/BP_PCWidget.BP_PCWidget_C'"));
		ensure(Finder.Class);
		if (Finder.Class)
		{
			Widget = Finder.Class;
		}
	}
}

void AControllerPC::BeginPlay()
{
	CurrentWidget = CreateWidget(GetWorld(), Widget);
	CurrentWidget->AddToViewport();
}
