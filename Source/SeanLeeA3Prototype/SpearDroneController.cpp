// Fill out your copyright notice in the Description page of Project Settings.


#include "SpearDroneController.h"


ASpearDroneController::ASpearDroneController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SightRadius = 5000.0f;
	SightAge = 3.5f;
	LoseSightRadius = SightRadius + 30.0f;
	FieldOfView = 180.0f;
	
	SightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfiguration"));

	SightConfiguration->SightRadius = SightRadius;
	SightConfiguration->LoseSightRadius = LoseSightRadius;
	SightConfiguration->PeripheralVisionAngleDegrees = FieldOfView;
	SightConfiguration->SetMaxAge(SightAge);

	SightConfiguration->DetectionByAffiliation.bDetectEnemies = true;
	SightConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfiguration->DetectionByAffiliation.bDetectNeutrals = true;

	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")));
	
	GetPerceptionComponent()->SetDominantSense(*SightConfiguration->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfiguration);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ASpearDroneController::OnSensesUpdated);

	AttackCoolDown = 3.5f;
	bCanAttack = false;
	
	PossessedSpearDrone = nullptr;
	TargetPlayer = nullptr;
}


void ASpearDroneController::BeginPlay()
{
	Super::BeginPlay();

	if (!DroneBlackboard)
		return;
	if (!ensure(BehaviorTree))
		return;

	UseBlackboard(DroneBlackboard, BlackboardComponent);
	RunBehaviorTree(BehaviorTree);

	// set demo shoot mode
	if (PossessedSpearDrone->bDemoShoot)
	{
		BlackboardComponent->SetValueAsBool("DemoShoot", true);
	}
}


void ASpearDroneController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ASpearDrone* PossessedPawn = Cast<ASpearDrone>(InPawn);

	if (PossessedPawn)
	{
		PossessedSpearDrone = PossessedPawn;
		PossessedSpearDrone->IsHurtDelegate.AddDynamic(this, &ASpearDroneController::IsOnHurt);
	}
}


void ASpearDroneController::Tick(float DeltaSeconds)
{
	if (!PossessedSpearDrone->bIsActive)
		return;
	
	Super::Tick(DeltaSeconds);

	// update target player position
	if (TargetPlayer)
	{
		FVector PlayerPosition = TargetPlayer->GetActorLocation() + FVector(0,0,TargetPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 10);
		BlackboardComponent->SetValueAsVector("PlayerPosition", PlayerPosition);
	}
}


void ASpearDroneController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	// modifying original UpdateControlRotation from AIController to allow for change in Pitch

	if (PossessedSpearDrone) 
	{
		FRotator NewControlRotation = GetControlRotation();

		// Look toward focus
		const FVector FocalPoint = GetFocalPoint();
		if (FAISystem::IsValidLocation(FocalPoint))
		{
			NewControlRotation = (FocalPoint - PossessedSpearDrone->GetActorLocation()).Rotation();
		}
		else if (bSetControlRotationFromPawnOrientation)
		{
			NewControlRotation = PossessedSpearDrone->SpearDroneMesh->GetComponentRotation();
		}

		SetControlRotation(NewControlRotation);

		if (bUpdatePawn)
		{
			const FRotator CurrentPawnRotation = PossessedSpearDrone->SpearDroneMesh->GetComponentRotation();

			if (CurrentPawnRotation.Equals(NewControlRotation, 1e-3f) == false)
			{
				PossessedSpearDrone->FaceRotation(NewControlRotation, DeltaTime);
			}
		}
	}
}


FRotator ASpearDroneController::GetControlRotation() const
{
	if (PossessedSpearDrone)
	{
		return PossessedSpearDrone->SpearDroneMesh->GetComponentRotation();
	}

	return FRotator::ZeroRotator;
}


void ASpearDroneController::Alerted(APlayerCharacter* Target)
{
	if (TargetPlayer)
		return;
		
	TargetPlayer = Target;
	BlackboardComponent->SetValueAsBool("AgroOnPlayer", true);

	FVector PlayerPosition = TargetPlayer->GetActorLocation() + FVector(0,0,TargetPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 10);
	BlackboardComponent->SetValueAsVector("PlayerPosition", PlayerPosition);

	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ASpearDroneController::OnAttackTimerTimer, AttackCoolDown, false);
	
	PossessedSpearDrone->HealthBarComponent->SetVisibility(true);

	// play alerted sound
	PossessedSpearDrone->PlaySound(PossessedSpearDrone->SBAlert);
}


void ASpearDroneController::UnAlert()
{
	TargetPlayer = nullptr;

	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	bCanAttack = false;
			
	BlackboardComponent->ClearValue("AgroOnPlayer");

	// play unalerted sound
	PossessedSpearDrone->PlaySound(PossessedSpearDrone->SBUnalert);
}


bool ASpearDroneController::Attack()
{
	if (!PossessedSpearDrone->bIsActive)
		return false;
	
	if (bCanAttack)
	{
		if (PossessedSpearDrone)
		{
			PossessedSpearDrone->FireBullet();
			bCanAttack = false;

			GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ASpearDroneController::OnAttackTimerTimer, AttackCoolDown, false);

			return true;
		}
	}

	return false;
}


void ASpearDroneController::OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	if (!PossessedSpearDrone->bIsActive)
		return;

	// adapted from FIT2096 Week 9 Lab
	
	APawn* TemporaryPawn = Cast<APawn>(UpdatedActor);

	if (TemporaryPawn && TemporaryPawn->IsPlayerControlled())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			if (Cast<APlayerCharacter>(TemporaryPawn))
			{
				Alerted(Cast<APlayerCharacter>(TemporaryPawn));
			}
		}
		else
		{
			UnAlert();
		}
	}
}


void ASpearDroneController::IsOnHurt(AActor* DamageParent)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(DamageParent);
	
	if (Player)
	{
		Alerted(Player);
	}
}


void ASpearDroneController::OnAttackTimerTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);

	bCanAttack = true;
}
