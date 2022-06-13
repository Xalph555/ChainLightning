// Fill out your copyright notice in the Description page of Project Settings.


#include "THCannonSection.h"


ATHCannonSection::ATHCannonSection()
{
	SectionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SectionMesh"));
	RootComponent = SectionMesh;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(SectionMesh);

	CannonHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CannonHitBox"));
	CannonHitBox->SetupAttachment(CannonMesh);
	
	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarComponent->SetupAttachment(SectionMesh);
	
	// properties
	MaxHealth = 300.0f;
	CurrentHealth = MaxHealth;

	bCanBeDamaged = false;

	IsPhase3 = false;

	ProjectileSpawnOffset = FVector(200.0f, 0.0f, 0.0f);

	bIsActive = false;
}


void ATHCannonSection::BeginPlay()
{
	Super::BeginPlay();

	// Set up Health bar widget
	UEnemyHealthBarWidget* HealthBar = Cast<UEnemyHealthBarWidget>(HealthBarComponent->GetWidget());

	if (HealthBar)
	{
		HealthChangedDelegate.AddDynamic(HealthBar, &UEnemyHealthBarWidget::OnHealthChanged);
	}

	HealthBarComponent->SetVisibility(false);
}


void ATHCannonSection::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// rotate health bar to face player
	FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	FRotator RotatationAmount = (CameraLocation - HealthBarComponent->GetComponentLocation()).Rotation();
	
	HealthBarComponent->SetWorldRotation(RotatationAmount);
}


void ATHCannonSection::FireProjectile()
{
	FRotator SpawnRotation = CannonMesh->GetComponentRotation();
	
	FVector SpawnOffset = SpawnRotation.RotateVector(ProjectileSpawnOffset);
	FVector SpawnLocation = CannonMesh->GetComponentLocation() + SpawnOffset;

	// spawn collision handling override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ATHCannonProjectile* SpawnedProjectile = GetWorld()->SpawnActor<ATHCannonProjectile>(ProjectileRef, SpawnLocation, SpawnRotation, ActorSpawnParams);
	
	if (SpawnedProjectile)
	{
		SpawnedProjectile->SetParent(this);
	}

	// play shoot sound
	PlaySound(SBShoot);
}


void ATHCannonSection::DestroyCannon()
{
	bIsActive = false;
	HealthBarComponent->SetVisibility(false);
	
	IsDestroyedDelegate.Broadcast(this);

	// play destroyed sound
	PlaySound(SBDestroyed);
}


void ATHCannonSection::SetCanDamage(bool CanDamage)
{
	if (!bIsActive)
		return;
	
	if (!IsPhase3)
		return;
	
	bCanBeDamaged = CanDamage;
	HealthBarComponent->SetVisibility(CanDamage);
}


FVector ATHCannonSection::GetPawnViewLocation() const
{
	return Super::GetPawnViewLocation();
}


void ATHCannonSection::FaceRotation(FRotator NewControlRotation, float DeltaTime)
{
	if (!bIsActive)
		return;
	
	// how cannon section rotation works:
	//	- rotation yaw controls section rotation
	//	- rotation pitch controls cannon rotation
	
	FRotator CurrentSectionRotation = GetActorRotation();
	FRotator CurrentCannonRotation = CannonMesh->GetComponentRotation();
	
	FRotator SectionRotation = FRotator(CurrentSectionRotation.Pitch, NewControlRotation.Yaw, CurrentSectionRotation.Roll);
	FRotator CannonRotation = FRotator(NewControlRotation.Pitch, CurrentSectionRotation.Yaw, CurrentSectionRotation.Roll);
	
	// rotate Section
	SetActorRotation(UKismetMathLibrary::RInterpTo(CurrentSectionRotation, SectionRotation, DeltaTime, 8));

	// rotate Cannon
	CannonMesh->SetWorldRotation(UKismetMathLibrary::RInterpTo(CurrentCannonRotation, CannonRotation, DeltaTime, 8));
}


void ATHCannonSection::ApplyDamage_Implementation(float DamageAmount, float KnockBackAmount, AActor* DamageObject,
                                                  AActor* DamageParent)
{
	if (DamageParent == this)
		return;

	if (DamageParent->IsA(ATHCannonSection::StaticClass()))
		return;

	if (!bIsActive)
		return;
	
	if (!bCanBeDamaged)
		return;

	AddCurrentHealth(-DamageAmount);

	// play hurt sound
	PlaySound(SBHurt);

	// destroy when out of HP
	if (GetCurrentHealth() <= 0)
	{
		DestroyCannon();
	}
}


// setters and getters
// -------------------------------------------------
float ATHCannonSection::GetMaxHealth() const
{
	return MaxHealth;
}

void ATHCannonSection::SetMaxHealth(float Health)
{
	MaxHealth = Health;

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}

void ATHCannonSection::AddMaxHealth(float Health)
{
	MaxHealth += Health;

	if (MaxHealth < 0)
		MaxHealth = 0.0f;

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}

float ATHCannonSection::GetCurrentHealth() const
{
	return CurrentHealth;
}

void ATHCannonSection::SetCurrentHealth(float Health)
{
	CurrentHealth = Health;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}

void ATHCannonSection::AddCurrentHealth(float Health)
{
	CurrentHealth += Health;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}

