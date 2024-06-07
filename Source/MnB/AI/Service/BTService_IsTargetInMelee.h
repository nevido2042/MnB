// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IsTargetInMelee.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UBTService_IsTargetInMelee : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_IsTargetInMelee();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float MeleeRange = 2.5f;
};
