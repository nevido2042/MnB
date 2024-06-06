// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CalculateDistance.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UBTService_CalculateDistance : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_CalculateDistance();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float WantDistacne = 50.f;
};
