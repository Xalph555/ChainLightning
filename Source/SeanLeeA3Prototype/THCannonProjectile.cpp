// Fill out your copyright notice in the Description page of Project Settings.


#include "THCannonProjectile.h"


ATHCannonProjectile::ATHCannonProjectile()
{
	// adapted from First-Person template PrototypeProjectile

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = ProjectileMesh;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bShouldBounce = true;

	InitialProjectileSpeed = 1000.0f;
	MaxProjectileSpeed = 1000.0f;

	MovementComponent->InitialSpeed = InitialProjectileSpeed;
	MovementComponent->MaxSpeed = MaxProjectileSpeed;
}


void ATHCannonProjectile::BeginPlay()
{
	Super::BeginPlay();
}


void ATHCannonProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


