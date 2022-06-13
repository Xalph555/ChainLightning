// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_RotateToFaceBBEntry.h"
#include "THTurretRotateToTask.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API UTHTurretRotateToTask : public UBTTask_RotateToFaceBBEntry
{
	GENERATED_BODY()

private:
	/** cached Precision tangent value */
	float PrecisionDot;

	
public:
	float CalculateAngleDifferenceDot(const FVector& VectorA, const FVector& VectorB) const;
	
	virtual void PostInitProperties() override;
	virtual void PostLoad() override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	bool AtRotationLimit(FRotator CurrentRotation, FRotator DefaultRotation, float HorizontalLimit, float VerticalLimit);
};
