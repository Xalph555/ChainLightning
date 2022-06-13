// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IsActorCloseEnoughDecorator.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API UIsActorCloseEnoughDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float ThresholdDistance;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FBlackboardKeySelector TargetLocation;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
