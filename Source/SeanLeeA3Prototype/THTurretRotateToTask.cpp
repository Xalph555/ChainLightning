// Fill out your copyright notice in the Description page of Project Settings.


#include "THTurretRotateToTask.h"
#include "THTurretController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"


float UTHTurretRotateToTask::CalculateAngleDifferenceDot(const FVector& VectorA, const FVector& VectorB) const
{
	return (VectorA.IsNearlyZero() || VectorB.IsNearlyZero())
			? 1.f
			: VectorA.CosineAngle2D(VectorB);
}


void UTHTurretRotateToTask::PostInitProperties()
{
	Super::PostInitProperties();

	PrecisionDot = FMath::Cos(FMath::DegreesToRadians(Precision));
}


void UTHTurretRotateToTask::PostLoad()
{
	Super::PostLoad();

	PrecisionDot = FMath::Cos(FMath::DegreesToRadians(Precision));
}


/*// taken from parent BBTask_RotateToFaceBBEntry
namespace
{
	FORCEINLINE_DEBUGGABLE float CalculateAngleDifferenceDot(const FVector& VectorA, const FVector& VectorB)
	{
		
	}
}*/


EBTNodeResult::Type UTHTurretRotateToTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// modifying original BBTask_RotateToFaceBBEntry to check for rotation limit

	ATHTurretController* AIController = Cast<ATHTurretController>(OwnerComp.GetAIOwner());

	if (AIController == NULL || AIController->GetPawn() == NULL)
	{
		return EBTNodeResult::Failed;
	}

	FBTFocusMemory* MyMemory = (FBTFocusMemory*)NodeMemory;
	check(MyMemory);
	MyMemory->Reset();

	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	APawn* Pawn = AIController->GetPawn();
	const FVector PawnLocation = Pawn->GetActorLocation();
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();

	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		AActor* ActorValue = Cast<AActor>(KeyValue);

		if (ActorValue != NULL)
		{
			const float AngleDifference = CalculateAngleDifferenceDot(Pawn->GetActorForwardVector()
				, (ActorValue->GetActorLocation() - PawnLocation));
			
			if (AtRotationLimit(AIController->GetControlRotation(), AIController->DefaultLookDir.Rotation(), AIController->HorizontalRotationLimit, AIController->VerticalRotationLimit)
				|| AngleDifference >= PrecisionDot)
			{
				Result = EBTNodeResult::Succeeded;
			}
			else
			{
				AIController->SetFocus(ActorValue, EAIFocusPriority::Gameplay);
				MyMemory->FocusActorSet = ActorValue;
				MyMemory->bActorSet = true;
				//Result = EBTNodeResult::InProgress;
			}
		}
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		const FVector KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
		
		if (FAISystem::IsValidLocation(KeyValue))
		{
			const float AngleDifference = CalculateAngleDifferenceDot(Pawn->GetActorForwardVector()
				, (KeyValue - PawnLocation));

			if (AtRotationLimit(AIController->GetControlRotation(), AIController->DefaultLookDir.Rotation(), AIController->HorizontalRotationLimit, AIController->VerticalRotationLimit)
				|| AngleDifference >= PrecisionDot)
			{
				Result = EBTNodeResult::Succeeded;
			}
			else
			{
				AIController->SetFocalPoint(KeyValue, EAIFocusPriority::Gameplay);
				MyMemory->FocusLocationSet = KeyValue;
				//Result = EBTNodeResult::InProgress;
			}
		}
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
	{
		const FRotator KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Rotator>(BlackboardKey.GetSelectedKeyID());

		if (FAISystem::IsValidRotation(KeyValue))
		{
			const FVector DirectionVector = KeyValue.Vector();
			const float AngleDifference = CalculateAngleDifferenceDot(Pawn->GetActorForwardVector(), DirectionVector);

			if (AtRotationLimit(AIController->GetControlRotation(), AIController->DefaultLookDir.Rotation(), AIController->HorizontalRotationLimit, AIController->VerticalRotationLimit)
				|| AngleDifference >= PrecisionDot)
			{
				Result = EBTNodeResult::Succeeded;
			}
			else
			{
				const FVector FocalPoint = PawnLocation + DirectionVector * 10000.0f;
				// set focal somewhere far in the indicated direction
				AIController->SetFocalPoint(FocalPoint, EAIFocusPriority::Gameplay);
				MyMemory->FocusLocationSet = FocalPoint;
				//Result = EBTNodeResult::InProgress;
			}
		}
	}

	return Result;
}


void UTHTurretRotateToTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// modifying original BBTask_RotateToFaceBBEntry to check for rotation limit

	ATHTurretController* AIController = Cast<ATHTurretController>(OwnerComp.GetAIOwner());

	if (AIController == NULL || AIController->GetPawn() == NULL)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	else
	{
		const FVector PawnDirection = AIController->GetPawn()->GetActorForwardVector();				
		const FVector FocalPoint = AIController->GetFocalPointForPriority(EAIFocusPriority::Gameplay);

		if (FocalPoint != FAISystem::InvalidLocation)
		{
			if (AtRotationLimit(AIController->GetControlRotation(), AIController->DefaultLookDir.Rotation(), AIController->HorizontalRotationLimit, AIController->VerticalRotationLimit)
				|| CalculateAngleDifferenceDot(PawnDirection, FocalPoint - AIController->GetPawn()->GetActorLocation()) >= PrecisionDot)
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


bool UTHTurretRotateToTask::AtRotationLimit(FRotator CurrentRotation, FRotator DefaultRotation, float HorizontalLimit, float VerticalLimit)
{
	// check yaw
	if (UKismetMathLibrary::NearlyEqual_FloatFloat(CurrentRotation.Yaw, DefaultRotation.Yaw - HorizontalLimit)
		|| UKismetMathLibrary::NearlyEqual_FloatFloat(CurrentRotation.Yaw, DefaultRotation.Yaw + HorizontalLimit))
	{
		UE_LOG(LogTemp, Warning, TEXT("Turret Rotation Task success - Yaw"));
		
		return true;
	}

	// check pitch
	if (UKismetMathLibrary::NearlyEqual_FloatFloat(CurrentRotation.Pitch, DefaultRotation.Pitch - VerticalLimit)
		|| UKismetMathLibrary::NearlyEqual_FloatFloat(CurrentRotation.Pitch, DefaultRotation.Pitch + VerticalLimit))
	{
		UE_LOG(LogTemp, Warning, TEXT("Turret Rotation Task success - Pitch"));
		
		return true;
	}
	
	return false;
}
