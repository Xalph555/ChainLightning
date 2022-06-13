// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GearGrunt.h"
#include "NavigationSystem.h"
#include "PlayerCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GearGruntController.generated.h"

/**
 * 
 */


UCLASS()
class SEANLEEA3PROTOTYPE_API AGearGruntController : public AAIController
{
	GENERATED_BODY()

	
public:
	AGearGruntController();
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGruntController|SightConfig")
    float SightRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGruntController|SightConfig")
    float SightAge;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGruntController|SightConfig")
    float LoseSightRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGruntController|SightConfig")
    float FieldOfView;;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGruntController|SightConfig")
    UAISenseConfig_Sight* SightConfiguration;

    UPROPERTY(EditDefaultsOnly, Category="GearGruntController|BlackBoard")
    UBlackboardData* GruntBlackboard;
	UPROPERTY(EditDefaultsOnly, Category="GearGruntController|BlackBoard")
	UBlackboardComponent* BlackboardComponent;
	UPROPERTY(EditDefaultsOnly, Category="GearGruntController|BlackBoard")
    UBehaviorTree* BehaviorTree;
	
    UNavigationSystemV1* NavigationSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGruntController|Attack")
    float AlertRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GearGruntController|Attack")
	float AttackCoolDown;
	
	AGearGrunt* PossessedGearGrunt;
    APlayerCharacter* TargetPlayer;


protected:
	FTimerHandle AttackTimerHandle;

	bool bCanAttack;
	

public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;

	virtual FRotator GetControlRotation() const override;

	void Alerted(APlayerCharacter* Target);
	void AlertSurroundingAllies();
	
	void Attack();

	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus);
	UFUNCTION()
	void IsOnHurt(AActor* DamageParent);

	void OnAttackTimerTimer();
};
