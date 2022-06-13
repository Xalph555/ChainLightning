// Fill out your copyright notice in the Description page of Project Settings.


#include "THTurretController.h"

#include "BrainComponent.h"


ATHTurretController::ATHTurretController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SightRadius = 4000.0f;
	SightAge = 3.5f;
	LoseSightRadius = SightRadius + 30.0f;
	FieldOfView = 45.0f;
	
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
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ATHTurretController::OnSensesUpdated);

	AttackCoolDown = 2.0f;
	bCanAttack = false;

	VerticalRotationLimit = 45.0f;
	HorizontalRotationLimit = 45.0f;
	
	DefaultLookPoint = FVector::Zero();
	DefaultLookDir = FVector::Zero();
	
	PossesedTurret = nullptr;
}


void ATHTurretController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ATHTurret* PossessedPawn = Cast<ATHTurret>(InPawn);

	if (PossessedPawn)
	{
		PossesedTurret = PossessedPawn;
		PossesedTurret->IsHurtDelegate.AddDynamic(this, &ATHTurretController::IsOnHurt);
		PossesedTurret->IsDestroyedDelegate.AddDynamic(this, &ATHTurretController::OnTurretDestroyed);

		// set DefaultLookPoint
		UpdateDefaults();

		// logging
		UE_LOG(LogTemp, Warning, TEXT("Turret Controller (%s) Default look point: %s"), *GetActorLabel(), *DefaultLookPoint.ToString());
	}
}


void ATHTurretController::BeginPlay()
{
	Super::BeginPlay();

	if (!THTurretBlackboard)
		return;
	if (!ensure(BehaviorTree))
		return;

	UseBlackboard(THTurretBlackboard, BlackboardComponent);
	RunBehaviorTree(BehaviorTree);
}


void ATHTurretController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!PossesedTurret)
		return;

	// update default look point
	UpdateDefaults();
	
	// keep track of whether Boss's shield is active
	if (BlackboardComponent->GetValueAsBool("IsShieldActive"))
	{
		PossesedTurret->SetCanDamage(false);
	}
	else
	{
		PossesedTurret->SetCanDamage(true);
	}
}


void ATHTurretController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	// modifying original UpdateControlRotation from AIController to allow for change
	// in Pitch and restricted rotation
	
	APawn* const MyPawn = GetPawn();
	if (MyPawn) 
	{
		FRotator NewControlRotation = GetControlRotation();

		// Look toward focus
		const FVector FocalPoint = GetFocalPoint();
		if (FAISystem::IsValidLocation(FocalPoint))
		{
			NewControlRotation = (FocalPoint - MyPawn->GetPawnViewLocation()).Rotation();
		}
		else if (bSetControlRotationFromPawnOrientation)
		{
			NewControlRotation = MyPawn->GetActorRotation();
		}

		// restrict rotation
		UpdateDefaults();

		FRotator StartLookDir =  DefaultLookDir.Rotation();
		
		NewControlRotation.Yaw = FMath::Clamp(NewControlRotation.Yaw,
			(StartLookDir.Yaw - HorizontalRotationLimit),
			(StartLookDir.Yaw + HorizontalRotationLimit));
		
		NewControlRotation.Pitch = FMath::Clamp(NewControlRotation.Pitch,
			(StartLookDir.Pitch - VerticalRotationLimit),
			(StartLookDir.Pitch + VerticalRotationLimit));
		
		SetControlRotation(NewControlRotation);

		if (bUpdatePawn)
		{
			const FRotator CurrentPawnRotation = MyPawn->GetActorRotation();

			if (CurrentPawnRotation.Equals(NewControlRotation, 1e-3f) == false)
			{
				MyPawn->FaceRotation(NewControlRotation, DeltaTime);
			}
		}
	}
}


FRotator ATHTurretController::GetControlRotation() const
{
	if (PossesedTurret)
	{
		return PossesedTurret->GetActorRotation();
	}

	return FRotator::ZeroRotator;
}


void ATHTurretController::UpdateDefaults()
{
	if (PossesedTurret->bIsActive)
	{
		DefaultLookDir = PossesedTurret->GetActorLocation() - PossesedTurret->ParentLocation;
	}
	else
	{
		DefaultLookDir = (PossesedTurret->GetActorLocation() + FVector(0,0,-100)) - PossesedTurret->ParentLocation;
	}

	DefaultLookDir.Normalize();
	DefaultLookPoint = PossesedTurret->GetActorLocation() + DefaultLookDir;	
}


void ATHTurretController::ResetTurretRotation()
{
	UpdateDefaults();
	
	SetFocalPoint(DefaultLookPoint);
	PossesedTurret->SetActorRotation(DefaultLookDir.Rotation());
	PossesedTurret->UpdateHealthBarRotation();
}


void ATHTurretController::Alerted()
{
	BlackboardComponent->SetValueAsBool("AgroOnPlayer", true);
	
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ATHTurretController::OnAttackTimerTimer, AttackCoolDown, false);
	
	PossesedTurret->HealthBarComponent->SetVisibility(true);

	// play alerted sound
	PossesedTurret->PlaySound(PossesedTurret->SBAlert);
}


void ATHTurretController::UnAlert()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	bCanAttack = false;
			
	BlackboardComponent->ClearValue("AgroOnPlayer");
	
	// play unalerted sound
	PossesedTurret->PlaySound(PossesedTurret->SBUnalert);
}


bool ATHTurretController::Attack()
{
	if (!PossesedTurret->bIsActive)
		return false;
	
	if (bCanAttack)
	{
		if (PossesedTurret)
		{
			PossesedTurret->FireProjectile();
			bCanAttack = false;

			GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ATHTurretController::OnAttackTimerTimer, AttackCoolDown, false);

			return true;
		}
	}

	return false;
}


void ATHTurretController::OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	if (!PossesedTurret->bIsActive)
		return;

	// adapted from FIT2096 Week 9 Lab
	
	APawn* TemporaryPawn = Cast<APawn>(UpdatedActor);

	if (TemporaryPawn && TemporaryPawn->IsPlayerControlled())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			if (Cast<APlayerCharacter>(TemporaryPawn))
			{
				Alerted();
			}
		}
		else
		{
			UnAlert();
		}
	}
}


void ATHTurretController::IsOnHurt(AActor* DamageParent)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(DamageParent);
	
	if (Player)
	{
		Alerted();
	}
}


void ATHTurretController::OnTurretDestroyed(ATHTurret* Turret)
{
	DefaultLookPoint = PossesedTurret->GetActorLocation() - FVector(0, 0, 10);
	UnAlert();
}


void ATHTurretController::OnAttackTimerTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);

	bCanAttack = true;
}
