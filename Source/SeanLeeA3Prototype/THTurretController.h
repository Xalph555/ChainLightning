// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "THTurret.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "THTurretController.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API ATHTurretController : public AAIController
{
	GENERATED_BODY()


public:
	ATHTurretController();


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurretController|SightConfig")
	float SightRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurretController|SightConfig")
	float SightAge;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurretController|SightConfig")
	float LoseSightRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurretController|SightConfig")
	float FieldOfView;;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurretController|SightConfig")
	UAISenseConfig_Sight* SightConfiguration;

	UPROPERTY(EditDefaultsOnly, Category="THTurretController|BlackBoard")
	UBlackboardData* THTurretBlackboard;
	UPROPERTY(EditDefaultsOnly, Category="THTurretController|BlackBoard")
	UBlackboardComponent* BlackboardComponent;
	UPROPERTY(EditDefaultsOnly, Category="THTurretController|BlackBoard")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurretController|Attack")
	float AttackCoolDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurretController|Rotation")
	float VerticalRotationLimit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THTurretController|Rotation")
	float HorizontalRotationLimit;

	FVector DefaultLookPoint;
	FVector DefaultLookDir;

	ATHTurret* PossesedTurret;
	
	
protected:
	FTimerHandle AttackTimerHandle;
	bool bCanAttack;


public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;
	virtual FRotator GetControlRotation() const override;

	void UpdateDefaults();
	
	void ResetTurretRotation();

	void Alerted();
	void UnAlert();
	
	bool Attack();


protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus);
	
	UFUNCTION()
	void IsOnHurt(AActor* DamageParent);

	UFUNCTION()
	void OnTurretDestroyed(ATHTurret* Turret);

	void OnAttackTimerTimer();
};
