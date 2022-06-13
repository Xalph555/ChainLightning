// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Marble.h"
#include "MarbleGenerator.generated.h"


UCLASS()
class SEANLEEA3PROTOTYPE_API AMarbleGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMarbleGenerator();


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MarbleGenerator")
	UStaticMeshComponent* GeneratorMesh;

	UPROPERTY(EditDefaultsOnly, Category = "MarbleGenerator")
	TSubclassOf<class AMarble> MarbleRef;
	
	// sound effects
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundEffects")
	USoundBase* SBSpawn;

	
protected:
	AMarble* SpawnedMarbleRef;

	
public:	
	void SpawnMarble();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnMarbleDissolved();
};
