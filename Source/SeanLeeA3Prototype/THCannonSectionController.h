// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "THCannonSection.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "THCannonSectionController.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API ATHCannonSectionController : public AAIController
{
	GENERATED_BODY()


public:
	ATHCannonSectionController();

	
	UPROPERTY(EditDefaultsOnly, Category="THCannonSectionController|BlackBoard")
	UBlackboardData* THCannonBlackboard;
	UPROPERTY(EditDefaultsOnly, Category="THCannonSectionController|BlackBoard")
	UBlackboardComponent* BlackboardComponent;
	UPROPERTY(EditDefaultsOnly, Category="THCannonSectionController|BlackBoard")
	UBehaviorTree* BehaviorTree;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THCannonSectionController|Attack")
	float AttackCoolDown;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THCannonSectionController|Attack")
	float AttackCoolDownPhase3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "THCannonSectionController|Rotation")
	float VerticalRotationLimit;

	float StartVerticalRotation;
		
	ATHCannonSection* PossessedCannon;
	

protected:
	FTimerHandle AttackTimerHandle;
	bool bCanAttack;

	
public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;

	void BossStart();
	void ChangePhase();

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;
	virtual FRotator GetControlRotation() const override;

	bool Attack();
	
	
protected:
	virtual void BeginPlay() override;

	void OnAttackTimerTimer();
};
