// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChainLightingConnection.generated.h"


UCLASS(Abstract)
class SEANLEEA3PROTOTYPE_API AChainLightingConnection : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	AChainLightingConnection();
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* LightningMesh;
	
	// sound effects
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundEffects")
	USoundBase* SBActivate;
	
	AActor* Parent;

	
protected:
	FTimerHandle LifeTimeTimerHandle;

	
public:	
	// Called every frame
	virtual void Destroyed() override;
	
	
protected:
	virtual void OnLifeTimeTimer();
};
