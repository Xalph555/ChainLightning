// Fill out your copyright notice in the Description page of Project Settings.


#include "SpearDroneRotateToTask.h"


float USpearDroneRotateToTask::CalculateAngleDifferenceDot(const FVector& VectorA, const FVector& VectorB) const
{
	return (VectorA.IsNearlyZero() || VectorB.IsNearlyZero())
			? 1.f
			: VectorA.CosineAngle2D(VectorB);
}

void USpearDroneRotateToTask::PostInitProperties()
{
	Super::PostInitProperties();

	PrecisionDot = FMath::Cos(FMath::DegreesToRadians(Precision));
}


void USpearDroneRotateToTask::PostLoad()
{
	Super::PostLoad();

	PrecisionDot = FMath::Cos(FMath::DegreesToRadians(Precision));
}


/*// taken from parent BBTask_RotateToFaceBBEntry
namespace
{
	FORCEINLINE_DEBUGGABLE float CalculateAngleDifferenceDot(const FVector& VectorA, const FVector& VectorB)
	{
		return (VectorA.IsNearlyZero() || VectorB.IsNearlyZero())
			? 1.f
			: VectorA.CosineAngle2D(VectorB);
	}
}*/


EBTNodeResult::Type USpearDroneRotateToTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//AAIController* AIController = OwnerComp.GetAIOwner();
	ASpearDroneController* AIController = Cast<ASpearDroneController>(OwnerComp.GetAIOwner());
	ASpearDrone* SpearDrone = Cast<ASpearDrone>(AIController->GetPawn());

	if (AIController == NULL || SpearDrone == NULL)
	{
		return EBTNodeResult::Failed;
	}

	FBTFocusMemory* MyMemory = (FBTFocusMemory*)NodeMemory;
	check(MyMemory);
	MyMemory->Reset();

	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	
	const FVector PawnLocation = SpearDrone->GetActorLocation();
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();

	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		AActor* ActorValue = Cast<AActor>(KeyValue);

		if (ActorValue != NULL)
		{
			const float AngleDifference = CalculateAngleDifferenceDot(SpearDrone->SpearDroneMesh->GetForwardVector()
				, (ActorValue->GetActorLocation() - PawnLocation));
			
			if (AngleDifference >= PrecisionDot)
			{
				Result = EBTNodeResult::Succeeded;
			}
			else
			{
				AIController->SetFocus(ActorValue, EAIFocusPriority::Gameplay);
				MyMemory->FocusActorSet = ActorValue;
				MyMemory->bActorSet = true;
				Result = EBTNodeResult::InProgress;
			}
		}
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		const FVector KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
		
		if (FAISystem::IsValidLocation(KeyValue))
		{
			const float AngleDifference = CalculateAngleDifferenceDot(SpearDrone->SpearDroneMesh->GetForwardVector()
				, (KeyValue - PawnLocation));

			if (AngleDifference >= PrecisionDot)
			{
				Result = EBTNodeResult::Succeeded;
			}
			else
			{
				AIController->SetFocalPoint(KeyValue, EAIFocusPriority::Gameplay);
				MyMemory->FocusLocationSet = KeyValue;
				Result = EBTNodeResult::InProgress;
			}
		}
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
	{
		const FRotator KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Rotator>(BlackboardKey.GetSelectedKeyID());

		if (FAISystem::IsValidRotation(KeyValue))
		{
			const FVector DirectionVector = KeyValue.Vector();
			const float AngleDifference = CalculateAngleDifferenceDot(SpearDrone->SpearDroneMesh->GetForwardVector(), DirectionVector);

			if (AngleDifference >= PrecisionDot)
			{
				Result = EBTNodeResult::Succeeded;
			}
			else
			{
				const FVector FocalPoint = PawnLocation + DirectionVector * 10000.0f;
				// set focal somewhere far in the indicated direction
				AIController->SetFocalPoint(FocalPoint, EAIFocusPriority::Gameplay);
				MyMemory->FocusLocationSet = FocalPoint;
				Result = EBTNodeResult::InProgress;
			}
		}
	}

	return Result;
}


void USpearDroneRotateToTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//AAIController* AIController = OwnerComp.GetAIOwner();
	ASpearDroneController* AIController = Cast<ASpearDroneController>(OwnerComp.GetAIOwner());
	ASpearDrone* SpearDrone = Cast<ASpearDrone>(AIController->GetPawn());

	if (AIController == NULL || SpearDrone == NULL)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	else
	{
		const FVector PawnDirection = SpearDrone->SpearDroneMesh->GetForwardVector();				
		const FVector FocalPoint = AIController->GetFocalPointForPriority(EAIFocusPriority::Gameplay);

		if (FocalPoint != FAISystem::InvalidLocation)
		{
			if (CalculateAngleDifferenceDot(PawnDirection, FocalPoint - AIController->GetPawn()->GetActorLocation()) >= PrecisionDot)
			{
				CleanUp(*AIController, NodeMemory);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
		else
		{
			CleanUp(*AIController, NodeMemory);
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	}
}


void USpearDroneRotateToTask::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	FString KeyDesc = BlackboardKey.SelectedKeyName.ToString();
	Values.Add(FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *KeyDesc));

	//AAIController* AIController = OwnerComp.GetAIOwner();
	ASpearDroneController* AIController = Cast<ASpearDroneController>(OwnerComp.GetAIOwner());
	ASpearDrone* SpearDrone = Cast<ASpearDrone>(AIController->GetPawn());

	if (AIController != NULL && SpearDrone != NULL)
	{
		const FVector PawnDirection = SpearDrone->SpearDroneMesh->GetForwardVector();
		const FVector FocalPoint = AIController->GetFocalPointForPriority(EAIFocusPriority::Gameplay);

		if (FocalPoint != FAISystem::InvalidLocation)
		{
			const float CurrentAngleRadians = CalculateAngleDifferenceDot(PawnDirection, (FocalPoint - AIController->GetPawn()->GetActorLocation()));
			Values.Add(FString::Printf(TEXT("Current angle: %.2f"), FMath::RadiansToDegrees(FMath::Acos(CurrentAngleRadians))));
		}
		else
		{
			Values.Add(TEXT("FocalPoint is an Invalid Location"));
		}
	}
	else
	{
		Values.Add(TEXT("Controller or Pawn is NULL"));
	}
}
