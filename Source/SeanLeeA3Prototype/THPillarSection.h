// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "THTurret.h"
#include "THPillarSection.generated.h"

/**
 * 
 */

class ATHPillarSection;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTHTurretSectionDestroyed, ATHPillarSection*, Section);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTHTurretSingleDestroyed, ATHTurret*, Turret);

UCLASS()
class SEANLEEA3PROTOTYPE_API ATHPillarSection : public AEnemyBase
{
	GENERATED_BODY()


public:
	ATHPillarSection();


	// delegates
	// ------------------------------------------------
	FOnTHTurretSectionDestroyed SectionDestroyedDelegate;
	FOnTHTurretSingleDestroyed TurretDestroyedDelegate;
	

	// class properties
	// -----------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THPillarSection")
	UStaticMeshComponent* SectionMesh;
	
	TArray<ATHTurret*> AttachedTurrets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THPillarSection|RotationPhase1")
	float MinRotationInterval;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THPillarSection|RotationPhase1")
	float MaxRotationInterval;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THPillarSection|RotationPhase1")
	float MinRotationAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THPillarSection|RotationPhase1")
	float MaxRotationAmount;

	FRotator TargetRotation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THPillarSection|RotationPhase2")
	float MinRotationIntervalPhase2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THPillarSection|RotationPhase2")
	float MaxRotationIntervalPhase2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THPillarSection|RotationPhase2")
	float MinRotationAmountPhase2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THPillarSection|RotationPhase2")
	float MaxRotationAmountPhase2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THPillarSection|SoundEffects")
	USoundBase* SBRotation;
	

protected:
	FTimerHandle RotationTimerHandle;


public:
	virtual void Tick(float DeltaSeconds) override;

	void BossStart();
	void ChangePhase();
	

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTurretDestroyed(ATHTurret* Turret);

	UFUNCTION()
	void OnRotationTimerTimer();
};
