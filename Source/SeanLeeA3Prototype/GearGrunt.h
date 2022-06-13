// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "EnemyBase.h"
#include "EnemyHealthBarWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GearGrunt.generated.h"


/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGearGruntHealthChanged, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGearGruntIsHurt, AActor*, DamageParent);


UCLASS()
class SEANLEEA3PROTOTYPE_API AGearGrunt : public AEnemyBase, public IDamageable
{
	GENERATED_BODY()

public:
	AGearGrunt();
	
	
	// delegates
	// ------------------------------------------------
	FOnGearGruntHealthChanged HealthChangedDelegate;
	FOnGearGruntIsHurt IsHurtDelegate;

	
	// class properties
	// -----------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGrunt")
	UCapsuleComponent* GearGruntCollisionBody;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGrunt")
	UStaticMeshComponent* GearGruntMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGrunt")
	UWidgetComponent* HealthBarComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGrunt")
	UFloatingPawnMovement* PawnMovementComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGrunt|Stats")
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGrunt|Stats")
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGrunt|Attack")
	float AttackDamage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGrunt|Attack")
	float KnockBack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGrunt|Attack")
	float AttackRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGrunt|Attack")
	FVector AttackSourceOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGrunt|Movement")
	float MovementSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGrunt|SoundEffects")
	USoundBase* SBAttack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGrunt|SoundEffects")
	USoundBase* SBAlert;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGrunt|SoundEffects")
	USoundBase* SBHurt;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGrunt|SoundEffects")
	USoundBase* SBDeath;
	
	
public:
	virtual void Tick(float DeltaTime) override;

	void Attack();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GearGrunt")
	void ApplyDamage(float DamageAmount, float KnockBackAmount, AActor* DamageObject,
			AActor* DamageParent);
	virtual void ApplyDamage_Implementation(float DamageAmount, float KnockBackAmount, AActor* DamageObject,
		AActor* DamageParent) override;

	// setters and getters
	float GetMaxHealth() const;
	void SetMaxHealth(float Health);
	void AddMaxHealth(float Health);

	float GetCurrentHealth() const;
	void SetCurrentHealth(float Health);
	void AddCurrentHealth(float Health);

	virtual UPawnMovementComponent* GetMovementComponent() const override;
	

protected:
	virtual void BeginPlay() override;
};
