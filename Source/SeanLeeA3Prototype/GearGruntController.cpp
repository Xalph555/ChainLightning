// Fill out your copyright notice in the Description page of Project Settings.


#include "GearGruntController.h"


AGearGruntController::AGearGruntController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SightRadius = 1000.0f;
	SightAge = 3.5f;
	LoseSightRadius = SightRadius + 30.0f;
	FieldOfView = 48.0f;
	
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
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AGearGruntController::OnSensesUpdated);

	
	AlertRadius = 1200.0f;
	AttackCoolDown = 0.5f;
	bCanAttack = true;
	
	PossessedGearGrunt = nullptr;
	TargetPlayer = nullptr;
}


void AGearGruntController::BeginPlay()
{
	Super::BeginPlay();

	NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());

	if (!GruntBlackboard)
		return;
	if (!ensure(BehaviorTree))
		return;

	UseBlackboard(GruntBlackboard, BlackboardComponent);
	RunBehaviorTree(BehaviorTree);
}


void AGearGruntController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AGearGrunt* PossessedPawn = Cast<AGearGrunt>(InPawn);

	if (PossessedPawn)
	{
		PossessedGearGrunt = PossessedPawn;
		PossessedGearGrunt->IsHurtDelegate.AddDynamic(this, &AGearGruntController::IsOnHurt);
	}
}


void AGearGruntController::Tick(float DeltaSeconds)
{
	if (!PossessedGearGrunt->bIsActive)
		return;
	
	Super::Tick(DeltaSeconds);

	// update target player position
	if (TargetPlayer)
	{
		BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
	}
}


FRotator AGearGruntController::GetControlRotation() const
{
	if (PossessedGearGrunt)
	{
		return FRotator(0, PossessedGearGrunt->GetActorRotation().Yaw, 0);
	}

	return FRotator::ZeroRotator;
}


void AGearGruntController::Alerted(APlayerCharacter* Target)
{
	if (TargetPlayer)
		return;
		
	TargetPlayer = Target;
	BlackboardComponent->SetValueAsBool("AgroOnPlayer", true);
	BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());

	PossessedGearGrunt->HealthBarComponent->SetVisibility(true);

	// play alerted sound
	PossessedGearGrunt->PlaySound(PossessedGearGrunt->SBAlert);
}


void AGearGruntController::AlertSurroundingAllies()
{
	if (!PossessedGearGrunt->bIsActive)
		return;
	
	// adapted from FIT2096 Week 6 Lab
	FVector Location = PossessedGearGrunt->GetActorLocation();
	FCollisionShape HitSphere = FCollisionShape::MakeSphere(AlertRadius);

	// debug - draw shape of damaging sphere
	//DrawDebugSphere(GetWorld(), Location, HitSphere.GetSphereRadius(), 50, FColor::Red, false, 2.0f, 0, 0);

	TArray<FHitResult> OutHits;
	
	if (GetWorld()->SweepMultiByChannel(OutHits, Location, Location, FQuat::Identity, ECC_WorldStatic, HitSphere))
	{
		for(auto& Hit: OutHits)
		{
			AGearGrunt* HitGrunt = Cast<AGearGrunt>(Hit.GetActor());

			if (HitGrunt)
			{
				AGearGruntController* HitGruntController = Cast<AGearGruntController>(HitGrunt->GetController());
				
				if (HitGruntController)
				{
					// logging
					UE_LOG(LogTemp, Display, TEXT("GearGrunt (%s) has alerted ally (%s) | Location: %s | Ally Location %s"),
						*GetActorLabel(), *HitGrunt->GetActorLabel(), *PossessedGearGrunt->GetActorLocation().ToString(), *HitGrunt->GetActorLocation().ToString());
					
					HitGruntController->Alerted(TargetPlayer);
				}
			}
		}
	}
}


void AGearGruntController::Attack()
{
	if (!PossessedGearGrunt->bIsActive)
		return;
	
	if (bCanAttack)
	{
		if (PossessedGearGrunt)
		{
			PossessedGearGrunt->Attack();
			bCanAttack = false;

			GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AGearGruntController::OnAttackTimerTimer, AttackCoolDown, false);
		}
	}
}


void AGearGruntController::OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	if (!PossessedGearGrunt->bIsActive)
		return;
	
	if (TargetPlayer)
		return;
	
	// adapted from FIT2096 Week 9 Lab
	APlayerCharacter* Player = Cast<APlayerCharacter>(UpdatedActor);
	
	if (Player)
	{
		Alerted(Player);
		AlertSurroundingAllies();	
	}
}


void AGearGruntController::IsOnHurt(AActor* DamageParent)
{
	// when hurt, alert others to call for help
	APlayerCharacter* Player = Cast<APlayerCharacter>(DamageParent);
	
	if (Player)
	{
		Alerted(Player);
		AlertSurroundingAllies();	
	}
}


void AGearGruntController::OnAttackTimerTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);

	bCanAttack = true;
}
