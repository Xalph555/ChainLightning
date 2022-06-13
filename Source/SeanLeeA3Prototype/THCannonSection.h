// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "EnemyBase.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "EnemyHealthBarWidget.h"
#include "THCannonProjectile.h"
#include "THCannonSection.generated.h"

class ATHCannonSection;

/**
 * 
 */


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTHCannonSectionHealthChanged, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTHCannonSectionDestroyed, ATHCannonSection*, CannonSection);


UCLASS()
class SEANLEEA3PROTOTYPE_API ATHCannonSection : public AEnemyBase, public IDamageable
{
	GENERATED_BODY()


public:
	ATHCannonSection();

	// delegates
	// ------------------------------------------------
	FOnTHCannonSectionHealthChanged HealthChangedDelegate;
	FOnTHCannonSectionDestroyed IsDestroyedDelegate;

	
	// class properties
	// -----------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THCannonSection")
	UStaticMeshComponent* SectionMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THCannonSection")
	UStaticMeshComponent* CannonMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THCannonSection")
	UBoxComponent* CannonHitBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THCannonSection")
	UWidgetComponent* HealthBarComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THCannonSection|Stats")
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THCannonSection|Stats")
	float CurrentHealth;
	
	bool bCanBeDamaged;

	bool IsPhase3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THCannonSection|Projectile")
	TSubclassOf<class ATHCannonProjectile> ProjectileRef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THCannonSection|Projectile")
	FVector ProjectileSpawnOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THCannonSection|SoundEffects")
	USoundBase* SBDamageableStateTransition;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THCannonSection|SoundEffects")
	USoundBase* SBShoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THCannonSection|SoundEffects")
	USoundBase* SBHurt;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THCannonSection|SoundEffects")
	USoundBase* SBDestroyed;

	
public:
	virtual void Tick(float DeltaSeconds) override;

	void FireProjectile();

	void DestroyCannon();

	void SetCanDamage(bool CanDamage);

	virtual FVector GetPawnViewLocation() const override;
	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "THCannonSection")
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
