// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "THCannonProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API ATHCannonProjectile : public AEnemyProjectile
{
	GENERATED_BODY()

public:
	ATHCannonProjectile();

	
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THCannonProjectile")
	float InitialProjectileSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THCannonProjectile")
	float MaxProjectileSpeed;


public:
	virtual void Tick(float DeltaSeconds) override;

	
protected:
	virtual void BeginPlay() override;
	
};
