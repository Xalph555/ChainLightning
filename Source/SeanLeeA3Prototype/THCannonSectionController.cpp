// Fill out your copyright notice in the Description page of Project Settings.


#include "THCannonSectionController.h"


ATHCannonSectionController::ATHCannonSectionController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AttackCoolDown = 20.0f;
	AttackCoolDownPhase3 = 12.0f;

	VerticalRotationLimit = 50.0f;

	StartVerticalRotation = 0.0f;

	PossessedCannon = nullptr;
}


void ATHCannonSectionController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ATHCannonSection* PossessedPawn = Cast<ATHCannonSection>(InPawn);

	if (PossessedPawn)
	{
		PossessedCannon = PossessedPawn;

		// set starting rotation
		StartVerticalRotation = PossessedCannon->CannonMesh->GetComponentRotation().Pitch;
	}
}


void ATHCannonSectionController::BeginPlay()
{
	Super::BeginPlay();

	if (!THCannonBlackboard)
		return;
	if (!ensure(BehaviorTree))
		return;

	UseBlackboard(THCannonBlackboard, BlackboardComponent);
	RunBehaviorTree(BehaviorTree);
}


void ATHCannonSectionController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// keep track of whether Boss's shield is active
	if (BlackboardComponent->GetValueAsBool("IsShieldActive"))
	{
		PossessedCannon->SetCanDamage(false);
	}
	else
	{
		PossessedCannon->SetCanDamage(true);
	}
}


void ATHCannonSectionController::BossStart()
{
	PossessedCannon->bIsActive = true;

	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ATHCannonSectionController::OnAttackTimerTimer, AttackCoolDown, false);
}


void ATHCannonSectionController::ChangePhase()
{
	if (!PossessedCannon->IsPhase3)
	{
		AttackCoolDown = AttackCoolDownPhase3;

		PossessedCannon->IsPhase3 = true;
		PossessedCannon->SetCanDamage(true);

		// play transition sound
		PossessedCannon->PlaySound(PossessedCannon->SBDamageableStateTransition);	
	}
}


void ATHCannonSectionController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	// modifying original UpdateControlRotation from AIController to allow for change
	// in Pitch and restricted rotation
	
	if (PossessedCannon) 
	{
		if (!PossessedCannon->bIsActive)
			return;
		
		FRotator NewControlRotation = GetControlRotation();

		// Look toward focus
		const FVector FocalPoint = GetFocalPoint();
		if (FAISystem::IsValidLocation(FocalPoint))
		{
			NewControlRotation = (FocalPoint - PossessedCannon->GetPawnViewLocation()).Rotation();
		}

		// restrict rotation
		NewControlRotation.Pitch = FMath::Clamp(NewControlRotation.Pitch, (StartVerticalRotation - VerticalRotationLimit), (StartVerticalRotation + VerticalRotationLimit));

		//UE_LOG(LogTemp, Warning, TEXT("New Cannon Rotation: %s"), *NewControlRotation.ToString());
		
		SetControlRotation(NewControlRotation);

		if (bUpdatePawn)
		{
			PossessedCannon->FaceRotation(NewControlRotation, DeltaTime);
		}
	}
}


FRotator ATHCannonSectionController::GetControlRotation() const
{
	return Super::GetControlRotation();
}


bool ATHCannonSectionController::Attack()
{
	if (!PossessedCannon->bIsActive)
		return false;
	
	if (bCanAttack)
	{
		if (PossessedCannon)
		{
			PossessedCannon->FireProjectile();
			bCanAttack = false;

			GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ATHCannonSectionController::OnAttackTimerTimer, AttackCoolDown, false);

			return true;
		}
	}

	return false;
}


void ATHCannonSectionController::OnAttackTimerTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);

	bCanAttack = true;
}
