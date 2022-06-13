// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChainLightingConnection.h"
#include "Damageable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CLSingle.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API ACLSingle : public AChainLightingConnection
{
	GENERATED_BODY()

public:
	ACLSingle();

	
	void SetBurst(AActor* SetParent, float Damage, float KnockBack, float DamageSizeR, float LifeTime);

	
protected:
	void DealDamage(float Damage, float KnockBack, float DamageSizeR);

	virtual void OnLifeTimeTimer() override;
};
