// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "Switch.h"
#include "ChainLightingConnection.h"
#include "LightningTriggeredSwitch.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API ALightningTriggeredSwitch : public ASwitch, public IDamageable
{
	GENERATED_BODY()


public:
	ALightningTriggeredSwitch();
	
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "LightingTriggeredSwitch")
	void ApplyDamage(float DamageAmount, float KnockBackAmount, AActor* DamageObject,
			AActor* DamageParent);
	virtual void ApplyDamage_Implementation(float DamageAmount, float KnockBackAmount, AActor* DamageObject,
		AActor* DamageParent) override;


protected:
	virtual void OnActivationIntervalTimer() override;
};
