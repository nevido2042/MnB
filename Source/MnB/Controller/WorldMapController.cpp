// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/WorldMapController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"

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

	WorldOption = CreateWidget(GetWorld(), WorldOptionAsset);
	WorldOption->AddToViewport();
	WorldOption->SetVisibility(ESlateVisibility::Hidden);
}

void AWorldMapController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

#include "LocalSaveLoad/WorldSaveGame.h"
#include "Kismet/GameplayStatics.h"
void AWorldMapController::OnPossess(APawn* aPawn)
{
	WorldSaveGame = Cast<UWorldSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("World"), 0));
	if (!WorldSaveGame)
	{
		WorldSaveGame = Cast<UWorldSaveGame>(UGameplayStatics::CreateSaveGameObject(UWorldSaveGame::StaticClass()));
	}
	ensure(WorldSaveGame);

	Super::OnPossess(aPawn);

	aPawn->SetActorLocation(WorldSaveGame->PlayerLocation);
	aPawn->SetActorRotation(WorldSaveGame->PlayerRotation);
}

void AWorldMapController::OnUnPossess()
{
	WorldSaveGame->PlayerLocation = GetPawn()->GetActorLocation();
	WorldSaveGame->PlayerRotation = GetPawn()->GetActorRotation();

	UGameplayStatics::SaveGameToSlot(WorldSaveGame, TEXT("World"), 0);

	Super::OnUnPossess();
}

void AWorldMapController::SetupInputComponent()
{
	Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
		EnhancedInputComponent->BindAction(ClickMoveAction, ETriggerEvent::Completed, this, &AWorldMapController::ClickMove);

        EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Completed, this, &AWorldMapController::Click);
    }
}

void AWorldMapController::ClickMove()
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, GetClickLocation());
}

void AWorldMapController::Click()
{
	FHitResult Hit;
	bool bHitSuccessful = false;

	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel3, true, Hit);

	if (bHitSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("dist %f"), FVector::Dist(GetPawn()->GetActorLocation(), Hit.Location));

		if (FVector::Dist(GetPawn()->GetActorLocation(), Hit.Location) > 300.f)
		{
			return;
		}

		WorldOption->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		WorldOption->SetVisibility(ESlateVisibility::Hidden);
	}

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
