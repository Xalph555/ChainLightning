// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SpearDroneController.h"
#include "SpearDroneAttackTask.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API USpearDroneAttackTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
