// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NonAttachable.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "SeanLeeA3Prototype.h"
#include "Shield.generated.h"


UCLASS()
class SEANLEEA3PROTOTYPE_API AShield : public AActor, public INonAttachable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShield();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	UStaticMeshComponent* ShieldMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	UBoxComponent* ShieldHitBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shield")
	bool bIsActive;

	// sound effects
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shield|SoundEffects")
	USoundBase* SBActivate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shield|SoundEffects")
	USoundBase* SBDeactivate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shield|SoundEffects")
	USoundBase* SBHit;

	AActor* Parent;

	
public:
	void PlaySound(USoundBase* Sound);
	
	void SetShieldActive(bool Active);
	
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "NonAttachable")
	void HitNonAttachable(UStaticMeshComponent* CollidingObject);
	virtual void HitNonAttachable_Implementation(UStaticMeshComponent* CollidingObject);
	
};
