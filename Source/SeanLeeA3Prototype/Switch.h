// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Switch.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTriggered);


UCLASS(Abstract)
class SEANLEEA3PROTOTYPE_API ASwitch : public AActor
{
	GENERATED_BODY()

	
public:	
	ASwitch();


	FOnTriggered TriggeredDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SwitchBase")
	UStaticMeshComponent* SwitchMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SwitchBase")
	UBoxComponent* SwitchHitBox;

	// properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SwitchBase")
	UMaterialInterface* BaseMaterial;

	UMaterialInstanceDynamic* SwitchMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SwitchBase")
	float ActivationInterval;
	
	UPROPERTY(EditInstanceOnly, Category = "SwitchBase")
	bool bIsMainSwitch;
	UPROPERTY(EditInstanceOnly, Category = "SwitchBase", meta = (EditCondition = "IsMainSwitch", EditConditionHides))
	TArray<ASwitch*> ConnectedSwitches;

	// sound effects
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SwitchBase|SoundEffects")
	USoundBase* SBActivate;

	
protected:
	bool bIsTriggered;
	
	FTimerHandle ActivationIntervalTimerHandle;

	
public:	
	virtual void MainSwitchCheck();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateMaterial();
	
	virtual void BindConnectedSwitches();
	
	virtual void OnActivationIntervalTimer();

	UFUNCTION()
	virtual void OnConnectedSwitchTriggered();
};
