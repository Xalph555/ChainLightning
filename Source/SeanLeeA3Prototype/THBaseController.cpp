// Fill out your copyright notice in the Description page of Project Settings.


#include "THBaseController.h"


ATHBaseController::ATHBaseController()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentPhase = 0;
	Phase2TurretThreshold = 4;
	DestroyedTurrets = 0;

	ShieldDeactivationInterval = 10.0f;
	ShieldCoolDown = 15.0f;
	ShieldPhase3CoolDown = 8.0f;

	PossessedBase = nullptr;
	TargetPlayer = nullptr;
}


void ATHBaseController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ATHBase* PossessedPawn = Cast<ATHBase>(InPawn);

	if (PossessedPawn)
	{
		PossessedBase = PossessedPawn;
		PossessedBase->OnBossStartDelegate.AddDynamic(this, &ATHBaseController::OnBossStart);
	}
}


void ATHBaseController::BeginPlay()
{
	Super::BeginPlay();

	if (!THBaseBlackboard)
		return;

	UseBlackboard(THBaseBlackboard, BlackboardComponent);
}


void ATHBaseController::Tick(float DeltaSeconds)
{
	if (!PossessedBase->bIsActive)
		return;
	
	Super::Tick(DeltaSeconds);

	// update target player position
	if (TargetPlayer)
	{
		FVector PlayerPosition = TargetPlayer->GetActorLocation() + FVector(0,0,TargetPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 10);
		BlackboardComponent->SetValueAsVector("PlayerPosition", PlayerPosition);
	}
}


void ATHBaseController::StartBossFight()
{
	// bind cannon section events
	PossessedBase->CannonSection->IsDestroyedDelegate.AddDynamic(this, &ATHBaseController::OnCannonDestroyed);

	// bind pillar section events
	for (auto& PillarSection: PossessedBase->PillarSections)
	{
		PillarSection->TurretDestroyedDelegate.AddDynamic(this, &ATHBaseController::OnTurretDestroyed);
		PillarSection->SectionDestroyedDelegate.AddDynamic(this, &ATHBaseController::OnPillarSectionDestroyed);
	}
	
	// activate turrets
	for(auto& PillarSection: PossessedBase->PillarSections)
	{
		PillarSection->BossStart();
	}

	// activate cannon
	Cast<ATHCannonSectionController>(PossessedBase->CannonSection->GetController())->BossStart();

	// activate shield
	PossessedBase->ActivateShield(true);
	BlackboardComponent->SetValueAsBool("IsShieldActive", true);
	
	// start shield CoolDown
	GetWorld()->GetTimerManager().SetTimer(ShieldCoolDownTimerHandle, this, &ATHBaseController::OnShieldCoolDownTimerTimer, ShieldCoolDown);
	
	// play boss start sound
	PossessedBase->PlaySound(PossessedBase->SBBossStart);
}


void ATHBaseController::OnPillarSectionDestroyed(ATHPillarSection* Section)
{
	// remove section from base array
	const int32 PillarIndex = PossessedBase->PillarSections.IndexOfByKey(Section);

	if (PillarIndex != INDEX_NONE)
	{
		PossessedBase->PillarSections.RemoveAtSwap(PillarIndex, 1, true);
		UE_LOG(LogTemp, Warning, TEXT("TH Boss Pillar Section Destroyed"));
	}

	// if all sections destroyed - change to final phase
	if (PossessedBase->PillarSections.Num() <= 0 && CurrentPhase != 3)
	{
		CurrentPhase = 3;
		ShieldCoolDown = ShieldPhase3CoolDown;
		Cast<ATHCannonSectionController>(PossessedBase->CannonSection->GetController())->ChangePhase();

		UE_LOG(LogTemp, Warning, TEXT("TH Boss transition to phase 3"));
	}
}


void ATHBaseController::OnTurretDestroyed(ATHTurret* Turret)
{
	// increment turret destroyed count and check if need to change phase
	DestroyedTurrets++;
	
	UE_LOG(LogTemp, Warning, TEXT("TH Boss Turret Destroyed"));

	if (DestroyedTurrets >= Phase2TurretThreshold && CurrentPhase != 2)
	{
		for(auto& PillarSection: PossessedBase->PillarSections)
		{
			CurrentPhase = 2;
			PillarSection->ChangePhase();
			
			UE_LOG(LogTemp, Warning, TEXT("TH Boss transition to phase 2"));
		}
	}
}


void ATHBaseController::OnCannonDestroyed(ATHCannonSection* CannonSection)
{
	// boss has been defeated
	PossessedBase->bIsActive = false;

	GetWorld()->GetTimerManager().ClearTimer(ShieldDeactivationTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ShieldCoolDownTimerHandle);

	// play destoyed sound
	PossessedBase->PlaySound(PossessedBase->SBBossDestroyed);

	BossDefeatedDelegate.Broadcast();

	UE_LOG(LogTemp, Warning, TEXT("TH Boss has been defeated"));
}


void ATHBaseController::OnBossStart(APlayerCharacter* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("TH Boss Fight Triggered in controller"));
	
	TargetPlayer = Player;
}


void ATHBaseController::OnShieldCoolDownTimerTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(ShieldCoolDownTimerHandle);

	// deactivate shield
	PossessedBase->ActivateShield(false);
	BlackboardComponent->ClearValue("IsShieldActive");

	GetWorld()->GetTimerManager().SetTimer(ShieldDeactivationTimerHandle, this, &ATHBaseController::OnShieldDeactivationTimerTimer, ShieldDeactivationInterval);

	UE_LOG(LogTemp, Warning, TEXT("TH BossShield Deactivation timer start"));
}


void ATHBaseController::OnShieldDeactivationTimerTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(ShieldDeactivationTimerHandle);

	// activate shield
	PossessedBase->ActivateShield(true);
	BlackboardComponent->SetValueAsBool("IsShieldActive", true);

	GetWorld()->GetTimerManager().SetTimer(ShieldCoolDownTimerHandle, this, &ATHBaseController::OnShieldCoolDownTimerTimer, ShieldCoolDown);

	UE_LOG(LogTemp, Warning, TEXT("TH BossShield Cooldown timer start"));
}
