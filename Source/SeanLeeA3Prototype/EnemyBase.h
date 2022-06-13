// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LevelGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyBase.generated.h"


UCLASS()
class SEANLEEA3PROTOTYPE_API AEnemyBase : public APawn
{
	GENERATED_BODY()

	
public:
	// Sets default values for this pawn's properties
	AEnemyBase();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "EnemyBase")
	bool bIsActive;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PlaySound(USoundBase* Sound, bool FixedLocation = false);

	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnGameOver();
};
