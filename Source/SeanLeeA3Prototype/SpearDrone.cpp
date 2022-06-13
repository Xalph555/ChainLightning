// Fill out your copyright notice in the Description page of Project Settings.


#include "SpearDrone.h"
#include "SpearDroneProjectile.h"


ASpearDrone::ASpearDrone()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	SpearDroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpearDroneMesh"));
	SpearDroneMesh->SetupAttachment(RootComponent);

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarComponent->SetupAttachment(RootComponent);


	// properties
	MaxHealth = 45.0f;
	CurrentHealth = MaxHealth;

	BulletSpawnOffset = FVector(80.0f, 0.0f, 0.0f);

	bDemoShoot = false;

	ShieldFront = nullptr;
	ShieldBack = nullptr;
}


void ASpearDrone::BeginPlay()
{
	Super::BeginPlay();

	// Set up Health bar widget
	UEnemyHealthBarWidget* HealthBar = Cast<UEnemyHealthBarWidget>(HealthBarComponent->GetWidget());

	if (HealthBar)
	{
		HealthChangedDelegate.AddDynamic(HealthBar, &UEnemyHealthBarWidget::OnHealthChanged);
	}

	HealthBarComponent->SetVisibility(false);
	
	
	// getting references to shields
	// get Front Shield
	TArray<UActorComponent*> TempShields = GetComponentsByTag(UChildActorComponent::StaticClass(), FName("FrontShield"));
	UChildActorComponent* ShieldTemp;

	if (TempShields.Num() > 0)
	{
		ShieldTemp = Cast<UChildActorComponent>(TempShields[0]);
		
		if (ShieldTemp && Cast<AShield>(ShieldTemp->GetChildActor()))
		{
			ShieldFront = Cast<AShield>(ShieldTemp->GetChildActor());
			ShieldFront->Parent = this;
			//UE_LOG(LogTemp, Display, TEXT("Spear Drone (%s) FrontShield: %s"), *GetActorLabel(), *ShieldFront->GetActorLabel());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Spear Drone (%s) could not find its Front Shield after casting"), *GetActorLabel());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Spear Drone (%s) could not find its Front Shield at all"), *GetActorLabel());
	}
	
	// get Back Shield
	TempShields = GetComponentsByTag(UChildActorComponent::StaticClass(), FName("BackShield"));

	if (TempShields.Num() > 0)
	{
		ShieldTemp = Cast<UChildActorComponent>(TempShields[0]);
		
		if (ShieldTemp && Cast<AShield>(ShieldTemp->GetChildActor()))
		{
			ShieldBack = Cast<AShield>(ShieldTemp->GetChildActor());
			ShieldBack->Parent = this;
			//UE_LOG(LogTemp, Display, TEXT("Spear Drone (%s) BackShield: %s"), *GetActorLabel(), *ShieldBack->GetActorLabel());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Spear Drone (%s) could not find its Back Shield after casting"), *GetActorLabel());
		}	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Spear Drone (%s) could not find its Back Shield at all"), *GetActorLabel());
	}
}


void ASpearDrone::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// rotate health bar to face player
	FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	FRotator RotatationAmount = (CameraLocation - HealthBarComponent->GetComponentLocation()).Rotation();
	
	HealthBarComponent->SetWorldRotation(RotatationAmount);
}


void ASpearDrone::FireBullet()
{
	FRotator SpawnRotation = SpearDroneMesh->GetComponentRotation();
	
	FVector SpawnOffset = SpawnRotation.RotateVector(BulletSpawnOffset);
	FVector SpawnLocation = GetActorLocation() + SpawnOffset;

	// spawn collision handling override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ASpearDroneProjectile* SpawnedBullet = GetWorld()->SpawnActor<ASpearDroneProjectile>(ProjectileRef, SpawnLocation, SpawnRotation, ActorSpawnParams);
	
	if (SpawnedBullet)
	{
		SpawnedBullet->SetParent(this);
	}

	// play shoot sound
	PlaySound(SBShoot);
}


void ASpearDrone::FaceRotation(FRotator NewControlRotation, float DeltaTime)
{
	if (bUseControllerRotationPitch || bUseControllerRotationYaw || bUseControllerRotationRoll)
	{
		//const FRotator CurrentRotation = GetActorRotation();
		const FRotator CurrentRotation = SpearDroneMesh->GetComponentRotation();

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

		SpearDroneMesh->SetWorldRotation(UKismetMathLibrary::RInterpTo(CurrentRotation, NewControlRotation, DeltaTime, 8));
		//SetActorRotation(UKismetMathLibrary::RInterpTo(CurrentRotation, NewControlRotation, DeltaTime, 8));
	}
}


void ASpearDrone::ApplyDamage_Implementation(float DamageAmount, float KnockBackAmount, AActor* DamageObject,
                                             AActor* DamageParent)
{
	if (DamageParent == this)
		return;

	if (DamageParent->IsA(ASpearDrone::StaticClass()))
		return;
	
	// check damage source is in front of shields
	// front shield check
	FVector DirToDamage = DamageObject->GetActorLocation() - ShieldFront->GetActorLocation();
	DirToDamage.Normalize();
	
	if (FVector::DotProduct(ShieldFront->GetActorForwardVector(), DirToDamage) > 0)
		return;
	
	// back shield check
	DirToDamage = DamageObject->GetActorLocation() - ShieldBack->GetActorLocation();
	DirToDamage.Normalize();
	
	if (FVector::DotProduct(ShieldBack->GetActorForwardVector(), DirToDamage) > 0)
		return;
	
	AddCurrentHealth(-DamageAmount);
	IsHurtDelegate.Broadcast(DamageParent);

	// play hurt sound
	PlaySound(SBHurt);

	HealthBarComponent->SetVisibility(true);

	// destroy when out of HP
	if (GetCurrentHealth() <= 0)
	{
		// play death sound
		PlaySound(SBDeath, true);
		
		Destroy();
	}
}


// setters and getters
// -------------------------------------------------
float ASpearDrone::GetMaxHealth() const
{
	return MaxHealth;
}

void ASpearDrone::SetMaxHealth(float Health)
{
	MaxHealth = Health;

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}

void ASpearDrone::AddMaxHealth(float Health)
{
	MaxHealth += Health;

	if (MaxHealth < 0)
		MaxHealth = 0.0f;

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}

float ASpearDrone::GetCurrentHealth() const
{
	return CurrentHealth;
}

void ASpearDrone::SetCurrentHealth(float Health)
{
	CurrentHealth = Health;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}

void ASpearDrone::AddCurrentHealth(float Health)
{
	CurrentHealth += Health;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}
