// Fill out your copyright notice in the Description page of Project Settings.


#include "THTurretProjectile.h"


ATHTurretProjectile::ATHTurretProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MovementSpeed = 2000.0f;

	Damage = 10.0f;
	KnockBack = 600.0f;
}


void ATHTurretProjectile::BeginPlay()
{
	Super::BeginPlay();
}


void ATHTurretProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// projectile moves in straight line in direction fired in
	FVector CurrentLocation = GetActorLocation();

	FVector ForwardDir = GetActorForwardVector();
	FVector Velocity = ForwardDir * MovementSpeed * DeltaSeconds;

	CurrentLocation += Velocity;

	SetActorLocation(CurrentLocation);
}


