// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SEANLEEA3PROTOTYPE_API IDamageable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damageable")
	void ApplyDamage(float DamageAmount, float KnockBackAmount, AActor* DamageObject, AActor* DamageParent = nullptr);
};
