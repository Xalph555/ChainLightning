// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Switch.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"


UCLASS()
class SEANLEEA3PROTOTYPE_API ADoor : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	ADoor();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	UStaticMeshComponent* DoorMesh;
	
	UPROPERTY(EditInstanceOnly, Category = "Door")
	ASwitch* SwitchRef;

	UPROPERTY(EditInstanceOnly, Category = "Door", meta = (MakeEditWidget))
	FVector EndLocationSetter;

	UPROPERTY(EditInstanceOnly, Category = "Door")
	float TravelTime;

	// sound effects
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Door|SoundEffects")
	USoundBase* SBOpen;

	
protected:
	FVector EndLocation;
	
	bool bIsOpening;
	//float CurrentLerpTime;
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSwitchTriggered();
};
