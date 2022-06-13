// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "EnemyBase.h"
#include "Shield.h"
#include "Components/WidgetComponent.h"
#include "EnemyHealthBarWidget.h"
#include "SpearDrone.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpearDroneHealthChanged, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpearDroneIsHurt, AActor*, DamageParent);


UCLASS()
class SEANLEEA3PROTOTYPE_API ASpearDrone : public AEnemyBase, public IDamageable
{
	GENERATED_BODY()


public:
	ASpearDrone();

	
	// delegates
	// ------------------------------------------------
	FOnSpearDroneHealthChanged HealthChangedDelegate;
	FOnSpearDroneIsHurt IsHurtDelegate;

	
	// class properties
	// -----------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpearDrone")
	UStaticMeshComponent* SpearDroneMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpearDrone")
	UWidgetComponent* HealthBarComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpearDrone|Stats")
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpearDrone|Stats")
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpearDrone|Projectile")
	TSubclassOf<class ASpearDroneProjectile> ProjectileRef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpearDrone|Projectile")
	FVector BulletSpawnOffset;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "SpearDrone|Demo")
	bool bDemoShoot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpearDrone|SoundEffects")
	USoundBase* SBAlert;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpearDrone|SoundEffects")
	USoundBase* SBUnalert;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpearDrone|SoundEffects")
	USoundBase* SBShoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpearDrone|SoundEffects")
	USoundBase* SBHurt;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpearDrone|SoundEffects")
	USoundBase* SBDeath;
	
	AShield* ShieldFront;
	AShield* ShieldBack;


public:
	virtual void Tick(float DeltaSeconds) override;
	
	void FireBullet();

	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SpearDrone")
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
