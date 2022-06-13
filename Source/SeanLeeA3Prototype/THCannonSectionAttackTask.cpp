// Fill out your copyright notice in the Description page of Project Settings.


#include "THCannonSectionAttackTask.h"



EBTNodeResult::Type UTHCannonSectionAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// adapted from FIT2096 Week 9 Lab
	
	UBehaviorTreeComponent* Component = &OwnerComp;

	if (!Component)
		return EBTNodeResult::Failed;

	ATHCannonSectionController* MyController = Cast<ATHCannonSectionController>(Component->GetOwner());

	if (!MyController)
		return EBTNodeResult::Failed;

	bool AttackSuccess = MyController->Attack();

	if (AttackSuccess)
		return EBTNodeResult::Succeeded;

	return EBTNodeResult::Failed;
}
