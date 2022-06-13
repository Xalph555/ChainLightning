// Fill out your copyright notice in the Description page of Project Settings.


#include "GearGrunt.h"


AGearGrunt::AGearGrunt()
{
	GearGruntCollisionBody = CreateDefaultSubobject<UCapsuleComponent>(TEXT("GearGruntCollisionBody"));
	GearGruntCollisionBody->SetCapsuleSize(30.0f, 65.0f);
	RootComponent = GearGruntCollisionBody;
	
	GearGruntMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GearGruntMesh"));
	GearGruntMesh->SetupAttachment(GearGruntCollisionBody);

	PawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMovementComponent"));
	PawnMovementComponent->UpdatedComponent = RootComponent;

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarComponent->SetupAttachment(RootComponent);
	
	MaxHealth = 80.0f;
	CurrentHealth = MaxHealth;
	
	AttackDamage = 5.0f;
	KnockBack = 800.0f;
	AttackRadius = 60.0f;
	AttackSourceOffset = FVector(80.0f, 0.0f, 40.0f);

	MovementSpeed = 350.0f;
	PawnMovementComponent->MaxSpeed = MovementSpeed;
}


void AGearGrunt::BeginPlay()
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


void AGearGrunt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// rotate health bar to face player
	FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	FRotator RotatationAmount = (CameraLocation - HealthBarComponent->GetComponentLocation()).Rotation();
	
	HealthBarComponent->SetWorldRotation(RotatationAmount);
}


void AGearGrunt::Attack()
{
	// logging
	UE_LOG(LogTemp, Warning, TEXT("Gear Grunt (%s) is attacking"), *GetActorLabel());

	// adapted from FIT2096 Week 6 Lab
	
	FRotator Rotation = GetActorRotation();
	FVector Location = GetActorLocation() + Rotation.RotateVector(AttackSourceOffset);
	
	FCollisionShape HitSphere = FCollisionShape::MakeSphere(AttackRadius);

	// debug - draw shape of damaging sphere
	DrawDebugSphere(GetWorld(), Location, HitSphere.GetSphereRadius(), 50, FColor::Red, false, 1.0f, 0, 0);

	TArray<FHitResult> OutHits;
	
	if (GetWorld()->SweepMultiByChannel(OutHits, Location, Location, FQuat::Identity, ECC_WorldStatic, HitSphere))
	{
		for(auto& Hit: OutHits)
		{
			if (UKismetSystemLibrary::DoesImplementInterface(Hit.GetActor(), UDamageable::StaticClass()))
			{
				// logging
				UE_LOG(LogTemp, Display, TEXT("Gear Grunt has dealt damage | Damage: %f | Hit Actor: %s | Location: %s | Rotation: %s"),
					AttackDamage, *Hit.GetActor()->GetActorLabel(), *Hit.GetActor()->GetActorLocation().ToString(), *Hit.GetActor()->GetActorRotation().ToString());
				
				// apply damage to all damagable actors
				IDamageable::Execute_ApplyDamage(Hit.GetActor(), AttackDamage, KnockBack, this, this);
			}
		}
	}

	// play attack sound
	PlaySound(SBAttack);
}


void AGearGrunt::ApplyDamage_Implementation(float DamageAmount, float KnockBackAmount, AActor* DamageObject,
	AActor* DamageParent)
{
	if (DamageParent == this)
		return;

	if (DamageParent->IsA(AGearGrunt::StaticClass()))
		return;

	// logging
	UE_LOG(LogTemp, Display, TEXT("Gear Grunt damaged | Damage: %f"), DamageAmount);

	// applying knockback
	FVector Direction = GetActorLocation() - DamageObject->GetActorLocation();
	Direction.Normalize();

	FVector KnockBackForce = Direction * KnockBackAmount;
	
	PawnMovementComponent->Velocity += KnockBackForce;
	
	AddCurrentHealth(-DamageAmount);
	IsHurtDelegate.Broadcast(DamageParent);

	// play hurt sound
	PlaySound(SBHurt);

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
float AGearGrunt::GetMaxHealth() const
{
	return MaxHealth;
}

void AGearGrunt::SetMaxHealth(float Health)
{
	MaxHealth = Health;

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}

void AGearGrunt::AddMaxHealth(float Health)
{
	MaxHealth += Health;

	if (MaxHealth < 0)
		MaxHealth = 0.0f;

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}

float AGearGrunt::GetCurrentHealth() const
{
	return CurrentHealth;
}

void AGearGrunt::SetCurrentHealth(float Health)
{
	CurrentHealth = Health;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}

void AGearGrunt::AddCurrentHealth(float Health)
{
	CurrentHealth += Health;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}

UPawnMovementComponent* AGearGrunt::GetMovementComponent() const
{
	return PawnMovementComponent;
}

