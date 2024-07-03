// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacter.h"
#include "WarriorAI.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API AWarriorAI : public AAICharacter
{
	GENERATED_BODY()
	

private:
	virtual void Die() override;
};
