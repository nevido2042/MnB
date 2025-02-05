// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MNB_API UHealth : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealth();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddCurrentHP(float value) { CurHP += value; }
	float GetCurrentHP(){ return CurHP; }
	float GetMaxHP() { return MaxHP; }
	void SetCurrentHP(float Value) { CurHP = Value; }
protected:
	float MaxHP = 3.f;
	float CurHP = MaxHP;
};
