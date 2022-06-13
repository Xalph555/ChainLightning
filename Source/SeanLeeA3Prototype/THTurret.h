// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "EnemyBase.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "THTurretProjectile.h"
#include "EnemyHealthBarWidget.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "THTurret.generated.h"

/**
 * 
 */

class ATHTurret;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTHTurretHealthChanged, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTHTurretIsHurt, AActor*, DamageParent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTHTurretDestroyed, ATHTurret*, Turret);


UCLASS()
class SEANLEEA3PROTOTYPE_API ATHTurret : public AEnemyBase, public IDamageable
{
	GENERATED_BODY()


public:
	ATHTurret();


	// delegates
	// ------------------------------------------------
	FOnTHTurretHealthChanged HealthChangedDelegate;
	FOnTHTurretIsHurt IsHurtDelegate;
	FOnTHTurretDestroyed IsDestroyedDelegate;
	

	// class properties
	// -----------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurret")
	UStaticMeshComponent* TurretBaseMesh;
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurret")
	// UStaticMeshComponent* TurretCannonMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurret")
	UBoxComponent* TurretHitBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurret")
	UWidgetComponent* HealthBarComponent;

	UNiagaraComponent* SmokeParticles;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurret|Stats")
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurret|Stats")
	float CurrentHealth;

	bool bCanBeDamaged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurret|Projectile")
	TSubclassOf<class ATHTurretProjectile> ProjectileRef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurret|Projectile")
	FVector ProjectileSpawnOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurret|SoundEffects")
	USoundBase* SBAlert;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurret|SoundEffects")
	USoundBase* SBUnalert;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurret|SoundEffects")
	USoundBase* SBShoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurret|SoundEffects")
	USoundBase* SBHurt;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurret|SoundEffects")
	USoundBase* SBDestroyed;
	
	FVector ParentLocation;

public:
	virtual void Tick(float DeltaSeconds) override;

	void ActivateTurret(bool Active);
	
	void FireProjectile();

	void DestroyTurret();

	void SetCanDamage(bool CanDamage);

	void UpdateHealthBarRotation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "THTurret")
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

	
protected:
	virtual void BeginPlay() override;
};
