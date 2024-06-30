// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MnBAIController.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EAIState : uint8
{
	LeftAttack UMETA(DisplayName = "LeftAttack"),
	RightAttack UMETA(DisplayName = "RightAttack"),

	LeftGuard UMETA(DisplayName = "LeftGuard"),
	RightGuard UMETA(DisplayName = "RightGuard")
};

UCLASS()
class MNB_API AMnBAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	explicit AMnBAIController(FObjectInitializer const& ObjectInitializer);
	ACharacter* GetTarget() const { return TargetCharacter; }

protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class AAICharacter* TargetCharacter = nullptr;

public:
	AAICharacter* GetTargetCharacter() { return TargetCharacter; }
	void ClearTarget() { TargetCharacter = nullptr; }

	void SearchTarget();
private:
	class UAISenseConfig_Sight* SightConfig;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

	UFUNCTION()
	void OnTargetForgottened(AActor* Actor);


	void CheckTargetIsDie();

	void RotateToTarget();
};
