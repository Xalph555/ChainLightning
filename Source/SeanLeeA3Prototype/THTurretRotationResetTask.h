// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "THTurretController.h"
#include "THTurretRotationResetTask.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API UTHTurretRotationResetTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
