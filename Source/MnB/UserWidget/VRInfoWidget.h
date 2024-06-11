// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VRInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UVRInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* UI_ActorInfo;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetActorInfoText(FText Info);
};
