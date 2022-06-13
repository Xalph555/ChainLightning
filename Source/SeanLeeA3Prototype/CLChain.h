// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChainLightingConnection.h"
#include "Damageable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CLChain.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API ACLChain : public AChainLightingConnection
{
	GENERATED_BODY()

public:
	ACLChain();

	
	void SetConnection(AActor* SetParent, float Damage, float DamageSizeR, float DamageSizeL, float LifeTime);


protected:
	void DealDamage(float Damage, float DamageSizeR, float DamageSizeL);

	virtual void OnLifeTimeTimer() override;
};
