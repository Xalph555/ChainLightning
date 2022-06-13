// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "LevelTransitionPad.generated.h"


UCLASS()
class SEANLEEA3PROTOTYPE_API ALevelTransitionPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelTransitionPad();

	UPROPERTY(EditInstanceOnly, Category = "LevelTransitionPad")
	FName TargetLevel;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LevelTransitionPad")
	UBoxComponent* TransitionHitBox;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPadOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
