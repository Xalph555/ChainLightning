// Fill out your copyright notice in the Description page of Project Settings.


#include "THTurret.h"


ATHTurret::ATHTurret()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	TurretBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBaseMesh"));
	TurretBaseMesh->SetupAttachment(RootComponent);

	// TurretCannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretCannonMesh"));
	// TurretCannonMesh->SetupAttachment(TurretBaseMesh);

	TurretHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TurretHitBox"));
	TurretHitBox->SetupAttachment(RootComponent);

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarComponent->SetupAttachment(RootComponent);

	SmokeParticles = nullptr;
	
	// properties
	MaxHealth = 160.0f;
	CurrentHealth = MaxHealth;

	bCanBeDamaged = false;
	
	ProjectileSpawnOffset = FVector(60.0f, 0.0f, 0.0f);

	bIsActive = false;

	ParentLocation = FVector::Zero();
}


void ATHTurret::BeginPlay()
{
	Super::BeginPlay();
	
	// Set up Health bar widget
	UEnemyHealthBarWidget* HealthBar = Cast<UEnemyHealthBarWidget>(HealthBarComponent->GetWidget());

	if (HealthBar)
	{
		HealthChangedDelegate.AddDynamic(HealthBar, &UEnemyHealthBarWidget::OnHealthChanged);
	}

	HealthBarComponent->SetVisibility(false);

	// get reference to particle component
	TArray<UActorComponent*> ParticleEffects = GetComponentsByTag(UNiagaraComponent::StaticClass(), FName("Smoke"));

	if (ParticleEffects.Num() > 0)
	{
		SmokeParticles = Cast<UNiagaraComponent>(ParticleEffects[0]);

		if (SmokeParticles)
		{
			UE_LOG(LogTemp, Warning, TEXT("Turret (%s) found its smoke particles"), *GetActorLabel());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Turret (%s) could not find its smoke particles"), *GetActorLabel());
		}
	}
}


void ATHTurret::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateHealthBarRotation();
}


void ATHTurret::ActivateTurret(bool Active)
{
	bIsActive = Active;
	HealthBarComponent->SetVisibility(Active);
}


void ATHTurret::FireProjectile()
{
	FRotator SpawnRotation = GetActorRotation();
	
	FVector SpawnOffset = SpawnRotation.RotateVector(ProjectileSpawnOffset);
	FVector SpawnLocation = GetActorLocation() + SpawnOffset;

	// spawn collision handling override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ATHTurretProjectile* SpawnedProjectile = GetWorld()->SpawnActor<ATHTurretProjectile>(ProjectileRef, SpawnLocation, SpawnRotation, ActorSpawnParams);
	
	if (SpawnedProjectile)
	{
		SpawnedProjectile->SetParent(this);
	}

	// play shoot sound
	PlaySound(SBShoot);
}


void ATHTurret::DestroyTurret()
{
	bIsActive = false;
	HealthBarComponent->SetVisibility(false);
	SmokeParticles->Activate();

	// play hurt sound
	PlaySound(SBDestroyed);
	
	IsDestroyedDelegate.Broadcast(this);
}


void ATHTurret::SetCanDamage(bool CanDamage)
{
	if (!bIsActive)
		return;
	
	bCanBeDamaged = CanDamage;
	HealthBarComponent->SetVisibility(CanDamage);
}


void ATHTurret::UpdateHealthBarRotation()
{
	// rotate health bar to face player
	FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	FRotator RotatationAmount = (CameraLocation - HealthBarComponent->GetComponentLocation()).Rotation();
	
	HealthBarComponent->SetWorldRotation(RotatationAmount);
}


void ATHTurret::ApplyDamage_Implementation(float DamageAmount, float KnockBackAmount, AActor* DamageObject,
                                           AActor* DamageParent)
{
	if (DamageParent == this)
		return;

	if (DamageParent->IsA(ATHTurret::StaticClass()))
		return;
	
	if (!bIsActive)
		return;
	
	if (!bCanBeDamaged)
		return;

	AddCurrentHealth(-DamageAmount);
	IsHurtDelegate.Broadcast(DamageParent);

	// play hurt sound
	PlaySound(SBHurt);

	// destroy when out of HP
	if (GetCurrentHealth() <= 0)
	{
		DestroyTurret();
	}
}


// setters and getters
// -------------------------------------------------
float ATHTurret::GetMaxHealth() const
{
	return MaxHealth;
}

void ATHTurret::SetMaxHealth(float Health)
{
	MaxHealth = Health;

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}

void ATHTurret::AddMaxHealth(float Health)
{
	MaxHealth += Health;

	if (MaxHealth < 0)
		MaxHealth = 0.0f;

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}

float ATHTurret::GetCurrentHealth() const
{
	return CurrentHealth;
}

void ATHTurret::SetCurrentHealth(float Health)
{
	CurrentHealth = Health;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}

void ATHTurret::AddCurrentHealth(float Health)
{
	CurrentHealth += Health;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}


