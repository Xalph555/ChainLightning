// Fill out your copyright notice in the Description page of Project Settings.


#include "SpearDroneAttackTask.h"


EBTNodeResult::Type USpearDroneAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// adapted from FIT2096 Week 9 Lab
	
	UBehaviorTreeComponent* Component = &OwnerComp;

	if (!Component)
		return EBTNodeResult::Failed;

	ASpearDroneController* MyController = Cast<ASpearDroneController>(Component->GetOwner());

	if (!MyController)
		return EBTNodeResult::Failed;

	bool AttackSuccess = MyController->Attack();

	if (AttackSuccess)
		return EBTNodeResult::Succeeded;

	return EBTNodeResult::Failed;
}
