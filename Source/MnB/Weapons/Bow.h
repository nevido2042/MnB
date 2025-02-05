// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapon.h"
#include "Bow.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API ABow : public AWeapon
{
	GENERATED_BODY()

	ABow();

protected:
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent = nullptr;

	//UPROPERTY(EditAnywhere)

	//class UAudioComponent* Audio = nullptr;

public:
	void PlayRecoilSound();
	
};
