// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacter.h"
#include "WorldNPC.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API AWorldNPC : public AAICharacter
{
	GENERATED_BODY()

public:
	AWorldNPC();
	
protected:
	UPROPERTY(EditAnywhere)
	class USphereComponent* Sphere = nullptr;
};
