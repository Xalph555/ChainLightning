// Fill out your copyright notice in the Description page of Project Settings.


#include "THPillarSection.h"


ATHPillarSection::ATHPillarSection()
{
	SectionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SectionMesh"));
	RootComponent = SectionMesh;
	
	MinRotationInterval = 5.0f;
	MaxRotationInterval = 15.0f;
	MinRotationAmount = 15.0f;
	MaxRotationAmount = 45.0f;

	TargetRotation = FRotator::ZeroRotator;
	
	MinRotationIntervalPhase2 = 3.0f;
	MaxRotationIntervalPhase2 = 8.0f;
	MinRotationAmountPhase2 = 20.0f;
	MaxRotationAmountPhase2 = 50.0f;
}


void ATHPillarSection::BeginPlay()
{
	Super::BeginPlay();
	
	TargetRotation = GetActorRotation();

	// add turrets to AttachedTurretsArray
	TArray<UActorComponent*> TempTurrets;
	GetComponents(UChildActorComponent::StaticClass(), TempTurrets);

	if (TempTurrets.Num() > 0)
	{
		for (auto& TurretTemp: TempTurrets)
		{
			UChildActorComponent* TurretComponent = Cast<UChildActorComponent>(TurretTemp);

			if (!TurretComponent)
				continue;

			if (Cast<ATHTurret>(TurretComponent->GetChildActor()))
			{
				AttachedTurrets.Add(Cast<ATHTurret>(TurretComponent->GetChildActor()));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("TH Pillar Section (%s) could not find its turrets after casting"), *GetActorLabel());
			}
		}

		// bind turret destroyed event and set parent
		for (auto& Turret: AttachedTurrets)
		{
			Turret->ParentLocation = GetActorLocation();
			Turret->IsDestroyedDelegate.AddDynamic(this, &ATHPillarSection::OnTurretDestroyed);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TH Pillar Section (%s) could not find its turrets at all"), *GetActorLabel());
	}
}


void ATHPillarSection::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetActorRotation(UKismetMathLibrary::RInterpTo(GetActorRotation(), TargetRotation, DeltaSeconds, 5));
}


void ATHPillarSection::BossStart()
{
	for (auto& Turret: AttachedTurrets)
	{
		Turret->ActivateTurret(true);
	}

	// start rotation timer
	float RotationInterval = FMath::RandRange(MinRotationInterval, MaxRotationInterval);
	GetWorld()->GetTimerManager().SetTimer(RotationTimerHandle, this, &ATHPillarSection::OnRotationTimerTimer, RotationInterval, false);
}


void ATHPillarSection::ChangePhase()
{
	MinRotationInterval = MinRotationIntervalPhase2;
	MaxRotationInterval = MaxRotationIntervalPhase2;
	MinRotationAmount = MinRotationAmountPhase2;
	MaxRotationAmount = MaxRotationAmountPhase2;
}


void ATHPillarSection::OnTurretDestroyed(ATHTurret* Turret)
{
	TurretDestroyedDelegate.Broadcast(Turret);

	// remove turret from array
	const int32 TurretIndex = AttachedTurrets.IndexOfByKey(Turret);

	if (TurretIndex != INDEX_NONE)
	{
		AttachedTurrets.RemoveAtSwap(TurretIndex, 1, true);

		// temp destroy function
		// Turret->Destroy();
	}
	
	// check if there are still active turrets on the pillar section
	if (AttachedTurrets.Num() <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);
		SectionDestroyedDelegate.Broadcast(this);
	}
}


void ATHPillarSection::OnRotationTimerTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);

	float Rotation = FMath::RandRange(MinRotationAmount, MaxRotationAmount);
	TargetRotation = GetActorRotation() + FRotator(0, Rotation, 0);
	
	UE_LOG(LogTemp, Warning, TEXT("Pillar Section (%s) new rotation target: %s"), *GetActorLabel(), *TargetRotation.ToString());
	
	float RotationInterval = FMath::RandRange(MinRotationInterval, MaxRotationInterval);
	GetWorld()->GetTimerManager().SetTimer(RotationTimerHandle, this, &ATHPillarSection::OnRotationTimerTimer, RotationInterval, false);

	// play rotation sound
	PlaySound(SBRotation);
}
