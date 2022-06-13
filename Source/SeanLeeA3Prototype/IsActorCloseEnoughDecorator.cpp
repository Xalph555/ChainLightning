// Fill out your copyright notice in the Description page of Project Settings.


#include "IsActorCloseEnoughDecorator.h"
#include "GearGruntController.h"
#include "BehaviorTree/BTFunctionLibrary.h"


bool UIsActorCloseEnoughDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                              uint8* NodeMemory) const
{
	UBehaviorTreeComponent* Component = &OwnerComp;

	if (!Component)
		return false;

	AGearGruntController* MyController = Cast<AGearGruntController>(Component->GetOwner());

	if (!MyController)
		return false;

	FVector Location = UBTFunctionLibrary::GetBlackboardValueAsVector(UBTNode::GetNodeInstance(OwnerComp, NodeMemory), TargetLocation);

	float Distance = FVector::Distance(MyController->PossessedGearGrunt->GetActorLocation(), Location);

	if (Distance <= ThresholdDistance)
	{
		UE_LOG(LogTemp, Display, TEXT("Actor is close enough: Decorator passed"))
		return true;
	}

	UE_LOG(LogTemp, Display, TEXT("Actor is close enough: Decorator failed"))
	return false;
}
