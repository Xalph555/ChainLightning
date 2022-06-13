// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyProjectile.h"
#include "THTurretProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API ATHTurretProjectile : public AEnemyProjectile
{
	GENERATED_BODY()


public:
	ATHTurretProjectile();

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurretProjectile")
	float MovementSpeed;


public:
	virtual void Tick(float DeltaSeconds) override;


protected:
	virtual void BeginPlay() override;
};
