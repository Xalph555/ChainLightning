// Fill out your copyright notice in the Description page of Project Settings.


#include "THTurretRotationResetTask.h"




EBTNodeResult::Type UTHTurretRotationResetTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// adapted from FIT2096 Week 9 Lab
	
	UBehaviorTreeComponent* Component = &OwnerComp;

	if (!Component)
		return EBTNodeResult::Failed;

	ATHTurretController* MyController = Cast<ATHTurretController>(Component->GetOwner());

	if (!MyController)
		return EBTNodeResult::Failed;

	MyController->ResetTurretRotation();
	
	return EBTNodeResult::Succeeded;
}
