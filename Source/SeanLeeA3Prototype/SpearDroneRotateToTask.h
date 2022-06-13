// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "SpearDrone.h"
#include "SpearDroneController.h"
#include "BehaviorTree/Tasks/BTTask_RotateToFaceBBEntry.h"
#include "SpearDroneRotateToTask.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API USpearDroneRotateToTask : public UBTTask_RotateToFaceBBEntry
{
	GENERATED_BODY()

	// modifying original BBTask_RotateToFaceBBEntry to accomodate for Spear Drone rotation behaviour

private:
	float PrecisionDot;

public:
	float CalculateAngleDifferenceDot(const FVector& VectorA, const FVector& VectorB) const;
	
	virtual void PostInitProperties() override;
	virtual void PostLoad() override;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
};
