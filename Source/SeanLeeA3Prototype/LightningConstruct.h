// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "GameFramework/Actor.h"
#include "SeanLeeA3Prototype.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LightningConstruct.generated.h"


UCLASS()
class SEANLEEA3PROTOTYPE_API ALightningConstruct : public AActor, public IDamageable
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	ALightningConstruct();

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LightningConstruct")
	UStaticMeshComponent* ConstructMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LightningConstruct")
	float MaxMaterialAggressiveness;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LightningConstruct")
	UMaterialInterface* BaseMaterial;
	
	UMaterialInstanceDynamic* ConstructMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightningConstruct")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightningConstruct")
	float CurrentHealth;

	// sound effects
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LightningConstruct|SoundEffects")
	USoundBase* SBGenerate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LightningConstruct|SoundEffects")
	USoundBase* SBHit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LightningConstruct|SoundEffects")
	USoundBase* SBDestroy;
	
	AActor* Parent;

	
protected:
	FTimerHandle LifeTimeTimerHandle;

	float MaxLifeTime;


public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void PlaySound(USoundBase* Sound, bool FixedLocation = false);
	
	void SetConnection(AActor* SetParent, float Length, float LifeTime);
	void BreakConstruct();

	virtual void Destroyed() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "LightningConstruct")
	void ApplyDamage(float DamageAmount, float KnockBackAmount, AActor* DamageObject,
			AActor* DamageParent);
	virtual void ApplyDamage_Implementation(float DamageAmount, float KnockBackAmount, AActor* DamageObject,
		AActor* DamageParent) override;

	
protected:
	void OnLifeTimeTimer();
};
