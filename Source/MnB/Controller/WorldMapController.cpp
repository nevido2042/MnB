// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/WorldMapController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AWorldMapController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}

	SetShowMouseCursor(true);

	FInputModeGameAndUI Mode;
	SetInputMode(Mode);

}

void AWorldMapController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FHitResult Hit;
	bool bHitSuccessful = false;

	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel3, true, Hit);

	if (bHitSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Town"));
	}

}

void AWorldMapController::SetupInputComponent()
{
	Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(ClickMoveAction, ETriggerEvent::Completed, this, &AWorldMapController::ClickMove);
    }
}

void AWorldMapController::ClickMove()
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, GetClickLocation());
}

FVector AWorldMapController::GetClickLocation()
{
	FHitResult Hit;
	bool bHitSuccessful = false;

	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		return Hit.Location;
	}

	return FVector::ZeroVector;
}
