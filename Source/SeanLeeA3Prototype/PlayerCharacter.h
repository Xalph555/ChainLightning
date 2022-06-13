// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Card.h"
#include "ChainLightingConnection.h"
#include "CLChain.h"
#include "CLSingle.h"
#include "LightningConstruct.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "SeanLeeA3Prototype.h"
#include "PlayerCharacter.generated.h"


// delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnManaChanged, float, CurrentMana, float, MaxMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCardCountChanged, int, CurrentCardCount, int, MaxCardCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConstructChanged, bool, IsWall);


UCLASS()
class SEANLEEA3PROTOTYPE_API APlayerCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()

	
public:
	// Sets default values for this character's properties
	APlayerCharacter();


	// delegates
	// ------------------------------------------------
	FOnHealthChanged HealthChangedDelegate;
	FOnManaChanged ManaChangedDelegate;

	FOnCardCountChanged CardCountChangedDelegate;
	FOnConstructChanged ConstructChangedDelegate;
	
	
	// class properties
	// -----------------------------------------------
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* FirstPersonMesh;
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	UStaticMeshComponent* VisibleMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* PlayerCamera;

	// is dead
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bIsDead;

	// health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Health")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Health")
	float HealthRegenRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Health")
	float HealthRegenTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Health")
	float HealthRegenDelay;
	
	// mana
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Mana")
	float MaxMana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Mana")
	float CurrentMana;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Mana")
	float ManaRegenRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Mana")
	float ManaRegenTime;
	
	// card properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Card")
	TSubclassOf<class ACard> CardProjectile;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Card")
	FVector CardThrowPointOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Card")
	int MaxCardCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Card")
	int CurrentCardCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Card")
	float CardThrowForce;

	// chain lightning properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|ChainLightning")
	TSubclassOf<class ACLSingle> ChainLightningSingle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "Abilities|ChainLightning")
	TSubclassOf<class ACLChain> ChainLightningChain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|ChainLightning")
	float CLBaseManaCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|ChainLightning")
	float CLScaleManaCost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|ChainLightning")
	float CLConnectionDist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|ChainLightning")
	float CLDamageLifeTime;

	// chain lightning properties - single
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|ChainLightning")
	float CLSingleDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|ChainLightning")
	float CLSingleRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|ChainLightning")
	float CLSingleKnockBack;

	// chain lightning properties - chain
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|ChainLightning")
	float CLChainDamageMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|ChainLightning")
	float CLChainDamageMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|ChainLightning")
	float CLChainRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|ChainLightning")
	float CLChainTraceRadiusThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|ChainLightning")
	float CLChainConnectionDistMin;

	// lightning construct properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|LightningConstruct")
	TSubclassOf<class ALightningConstruct> LightningConstructWall;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|LightningConstruct")
	TSubclassOf<class ALightningConstruct> LightningConstructBridge;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|LightningConstruct")
	float LCBaseManaCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|LightningConstruct")
	float LCScaleManaCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|LightningConstruct")
	float LCConnectionDist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|LightningConstruct")
	float LCLifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|LightningConstruct")
	bool bIsLCWall;
	
	// movement properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|MovementSpeed")
	float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|MovementSpeed")
	float SprintSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|MovementSpeed")
	bool bIsSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Jump")
	float JumpHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Jump")
	float FallingGravityScale;

	// sound effects
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundEffects")
	USoundBase* SBHurt;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundEffects")
	USoundBase* SBDeath;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundEffects")
	USoundBase* SBThrowCard;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundEffects")
	USoundBase* SBAbilityActivation;

	
protected:
	FTimerHandle HealthRegenTimerHandle;
	FTimerHandle HealthRegenDelayTimerHandle;
	bool bCanHealthRegen;
	
	FTimerHandle ManaRegenTimerHandle;

	UNiagaraComponent* LightningCircleParticles;
	
private:
	TArray<ACard*> ThrownCards;
	TArray<ALightningConstruct*> GeneratedConstructs;
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PlaySound(USoundBase* Sound);
	
	void ThrowCard();
	void DestroyCards();

	UFUNCTION()
	void OnCardDestroyed(AActor* DestroyedActor);
	
	void ChainLightning();
	bool IsNonConductiveObjectInPath(FVector StartPoint, FVector EndPoint);
	bool TraceForNonConductive(UWorld* World, TArray<AActor*>& ActorsToIgnore, const FVector& Start,
		const FVector& End, TArray<FHitResult>& OutHits, ECollisionChannel CollisionChannel);
	
	void ChangeConstruct();
	void LightningConstruct();
	void SpawnConstruct(FVector StartPoint, FVector EndPoint);
	
	void DestroyConstructs();
	UFUNCTION()
	void OnConstructDestroyed(AActor* DestroyedActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	void ApplyDamage(float DamageAmount, float KnockBackAmount, AActor* DamageObject,
			AActor* DamageParent);
	virtual void ApplyDamage_Implementation(float DamageAmount, float KnockBackAmount, AActor* DamageObject,
		AActor* DamageParent) override;

	void PlayerDeath();
	
	virtual void FellOutOfWorld(const UDamageType& dmgType) override;

	
	// setters and getters
	float GetMaxHealth() const;
	void SetMaxHealth(float Health);
	void AddMaxHealth(float Health);

	float GetCurrentHealth() const;
	void SetCurrentHealth(float Health);
	void AddCurrentHealth(float Health);
	
	float GetMaxMana() const;
	void SetMaxMana(float Mana);
	void AddMaxMana(float Mana);

	float GetCurrentMana() const;
	void SetCurrentMana(float Mana);
	void AddCurrentMana(float Mana);
	
	int GetMaxCardCount() const;
	void SetMaxCardCount(int Cards);
	void AddMaxCardCount(int Cards);
	
	int GetCurrentCardCount() const;
	void SetCurrentCardCount(int Cards);
	void AddCurrentCardCount(int Cards);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// timer callback functions
	void OnHealthRegenTimer();
	void OnHealthDelayTimer();
	
	void OnManaRegenTimer();
};
