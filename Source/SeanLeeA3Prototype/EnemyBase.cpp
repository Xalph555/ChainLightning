// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"




// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bIsActive = true;
}


// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	ALevelGameMode* LevelGameMode = Cast<ALevelGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (LevelGameMode)
	{
		LevelGameMode->GameOverDelegate.AddDynamic(this, &AEnemyBase::OnGameOver);
	}
}


// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AEnemyBase::PlaySound(USoundBase* Sound, bool FixedLocation)
{
	if (!Sound)
		return;

	if (FixedLocation)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Sound, GetActorLocation());
	}
	else
	{
		UGameplayStatics::SpawnSoundAttached(Sound, RootComponent);
	}
}


void AEnemyBase::FaceRotation(FRotator NewControlRotation, float DeltaTime)
{
	// modifying original FaceRotation from APawn to allow for smoother rotation
	
	// Only if we actually are going to use any component of rotation.
	if (bUseControllerRotationPitch || bUseControllerRotationYaw || bUseControllerRotationRoll)
	{
		const FRotator CurrentRotation = GetActorRotation();

		if (!bUseControllerRotationPitch)
		{
			NewControlRotation.Pitch = CurrentRotation.Pitch;
		}

		if (!bUseControllerRotationYaw)
		{
			NewControlRotation.Yaw = CurrentRotation.Yaw;
		}

		if (!bUseControllerRotationRoll)
		{
			NewControlRotation.Roll = CurrentRotation.Roll;
		}

#if ENABLE_NAN_DIAGNOSTIC
		if (NewControlRotation.ContainsNaN())
		{
			logOrEnsureNanError(TEXT("APawn::FaceRotation about to apply NaN-containing rotation to actor! New:(%s), Current:(%s)"), *NewControlRotation.ToString(), *CurrentRotation.ToString());
		}
#endif
		
		SetActorRotation(UKismetMathLibrary::RInterpTo(CurrentRotation, NewControlRotation, DeltaTime, 8));
	}
}


void AEnemyBase::OnGameOver()
{
	bIsActive = false;
}
