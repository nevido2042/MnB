#pragma once
#include "CoreMinimal.h"
#include "LoadingScreen.generated.h"

UCLASS()
class ULoadingScreenSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetLoadingScreenClass(TSubclassOf<UUserWidget> InWidgetClass);

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection);

protected:
	virtual void OnPreLoadMap(const FString& MapName);

private:
	TSubclassOf<UUserWidget> WidgetClass;
};