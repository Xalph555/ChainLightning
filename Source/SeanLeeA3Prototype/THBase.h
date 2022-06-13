// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "Shield.h"
#include "THCannonSection.h"
#include "THPillarSection.h"
#include "THBase.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTHBossStart, APlayerCharacter*, Player);


UCLASS()
class SEANLEEA3PROTOTYPE_API ATHBase : public AEnemyBase
{
	GENERATED_BODY()


public:
	ATHBase();

	
	// delegates
	// ------------------------------------------------
	UPROPERTY(BlueprintAssignable, Category = "THBase")
	FOnTHBossStart OnBossStartDelegate;

	
	// class properties
	// -----------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THBase")
	UStaticMeshComponent* PillarMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "THBase")
	UBoxComponent* BossStartTrigger;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THBase|SoundEffects")
	USoundBase* SBBossStart;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THBase|SoundEffects")
	USoundBase* SBBossDestroyed;
	
	AShield* Shield;

	
	ATHCannonSection* CannonSection;
	TArray<ATHPillarSection*> PillarSections;
	

public:
	virtual void Tick(float DeltaSeconds) override;

	void ActivateShield(bool Active);
	

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBossStartOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
