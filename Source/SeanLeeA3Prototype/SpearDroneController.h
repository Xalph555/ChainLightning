// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PlayerCharacter.h"
#include "SpearDrone.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SpearDroneController.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API ASpearDroneController : public AAIController
{
	GENERATED_BODY()


public:
	ASpearDroneController();


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpearDroneController|SightConfig")
	float SightRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpearDroneController|SightConfig")
	float SightAge;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpearDroneController|SightConfig")
	float LoseSightRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpearDroneController|SightConfig")
	float FieldOfView;;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpearDroneController|SightConfig")
	UAISenseConfig_Sight* SightConfiguration;

	UPROPERTY(EditDefaultsOnly, Category="SpearDroneController|BlackBoard")
	UBlackboardData* DroneBlackboard;
	UPROPERTY(EditDefaultsOnly, Category="SpearDroneController|BlackBoard")
	UBlackboardComponent* BlackboardComponent;
	UPROPERTY(EditDefaultsOnly, Category="SpearDroneController|BlackBoard")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpearDroneController|Attack")
	float AttackCoolDown;
	
	ASpearDrone* PossessedSpearDrone;
	APlayerCharacter* TargetPlayer;


protected:
	FTimerHandle AttackTimerHandle;
	bool bCanAttack;


public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;
	virtual FRotator GetControlRotation() const override;

	void Alerted(APlayerCharacter* Target);
	void UnAlert();
	
	bool Attack();


protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus);
	
	UFUNCTION()
	void IsOnHurt(AActor* DamageParent);

	void OnAttackTimerTimer();
};
