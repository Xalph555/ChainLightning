// Fill out your copyright notice in the Description page of Project Settings.


#include "GearGruntAttackTask.h"
#include "GearGruntController.h"


EBTNodeResult::Type UGearGruntAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// adapted from FIT2096 Week 9 Lab
	
	UBehaviorTreeComponent* Component = &OwnerComp;

	if (!Component)
		return EBTNodeResult::Failed;

	AGearGruntController* MyController = Cast<AGearGruntController>(Component->GetOwner());

	if (!MyController)
		return EBTNodeResult::Failed;

	MyController->Attack();

	return EBTNodeResult::Succeeded;
}
