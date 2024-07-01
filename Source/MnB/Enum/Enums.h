// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBlockedDirection :uint8
{
	Left,
	None,
	Right
};

UENUM(BlueprintType)
enum class EAttackDirection :uint8
{
	AttackLeft,
	AttackNone,
	AttackRight
};

UENUM(BlueprintType)
enum class EGuardDirection :uint8
{
	GuardLeft,
	GuardNone,
	GarudRight
};

UENUM(BlueprintType)
enum class ETeam :uint8
{
	ATeam,
	BTeam
};