// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "THBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "THCannonSectionController.h"
#include "THBaseController.generated.h"

/**
 * 
 */


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDefeated);


UCLASS()
class SEANLEEA3PROTOTYPE_API ATHBaseController : public AAIController
{
	GENERATED_BODY()


public:
	ATHBaseController();


	// delegates
	// ------------------------------------------------
	UPROPERTY(BlueprintAssignable, Category = "THBaseController")
	FOnBossDefeated BossDefeatedDelegate;

	
	// class properties
	// -----------------------------------------------
	UPROPERTY(EditDefaultsOnly, Category= "THBaseController|BlackBoard")
	UBlackboardData* THBaseBlackboard;
	UPROPERTY(EditDefaultsOnly, Category= "THBaseController|BlackBoard")
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "THBaseController|Phases")
	int CurrentPhase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "THBaseController|Phases")
	int Phase2TurretThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "THBaseController|Phases")
	int DestroyedTurrets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "THBaseController|Shield")
	float ShieldDeactivationInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "THBaseController|Shield")
	float ShieldCoolDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "THBaseController|Shield")
	float ShieldPhase3CoolDown;

	ATHBase* PossessedBase;
	APlayerCharacter* TargetPlayer;


protected:
	FTimerHandle ShieldCoolDownTimerHandle;
	FTimerHandle ShieldDeactivationTimerHandle;


public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "THBaseController")
	void StartBossFight();
	

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPillarSectionDestroyed(ATHPillarSection* Section);
	UFUNCTION()
	void OnTurretDestroyed(ATHTurret* Turret);
	UFUNCTION()
	void OnCannonDestroyed(ATHCannonSection* CannonSection);

	UFUNCTION()
	void OnBossStart(APlayerCharacter* Player);

	UFUNCTION()
	void OnShieldCoolDownTimerTimer();
	UFUNCTION()
	void OnShieldDeactivationTimerTimer();
};
