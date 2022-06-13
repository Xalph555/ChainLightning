// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Marble.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDissolve);

UCLASS()
class SEANLEEA3PROTOTYPE_API AMarble : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMarble();

	
	FOnDissolve DissolveDelegate;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Marble")
	UStaticMeshComponent* MarbleMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Marble")
	float DissolveTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LightningConstruct")
	UMaterialInterface* BaseMaterial;
	
	UMaterialInstanceDynamic* MarbleMaterial;

	// sound effects
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Marble|SoundEffects")
	USoundBase* SBDissolve;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marble")
	float MarbleLifeTime;

	
protected:
	FTimerHandle LifeTimeTimerHandle;
	FTimerHandle DissolveTimeHandle;

	bool IsDissolving;
	float CurrentDissolveAmount;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void OnLifeTimeTimer();
	void OnDissolveTimerTimer();
};
