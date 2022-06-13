// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyProjectile.h"
#include "SpearDroneProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API ASpearDroneProjectile : public AEnemyProjectile
{
	GENERATED_BODY()


public:
	ASpearDroneProjectile();
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpearDroneProjectile")
	float MovementSpeed;


public:
	virtual void Tick(float DeltaSeconds) override;


protected:
	virtual void BeginPlay() override;
};
