// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Marble.h"
#include "Switch.h"
#include "MarbleTriggeredSwitch.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API AMarbleTriggeredSwitch : public ASwitch
{
	GENERATED_BODY()


public:
	AMarbleTriggeredSwitch();

	
protected:
	AMarble* TriggeringMarble;

	
protected:
	virtual void BeginPlay() override;

	virtual void OnActivationIntervalTimer() override;

	UFUNCTION()
	void OnSwitchOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSwitchOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex);
};
