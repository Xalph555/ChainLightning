// Fill out your copyright notice in the Description page of Project Settings.


#include "SpearDroneProjectile.h"


ASpearDroneProjectile::ASpearDroneProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MovementSpeed = 2800.0f;

	Damage = 35.0f;
	KnockBack = 1500.0f;
}


void ASpearDroneProjectile::BeginPlay()
{
	Super::BeginPlay();
}


void ASpearDroneProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// projectile moves in straight line in direction fired in
	FVector CurrentLocation = GetActorLocation();

	FVector ForwardDir = GetActorForwardVector();
	FVector Velocity = ForwardDir * MovementSpeed * DeltaSeconds;

	CurrentLocation += Velocity;

	SetActorLocation(CurrentLocation);
}

