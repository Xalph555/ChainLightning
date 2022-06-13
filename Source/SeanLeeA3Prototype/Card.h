// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "SeanLeeA3Prototype.h"
#include "NonAttachable.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/WidgetComponent.h"
#include "Card.generated.h"


UCLASS()
class SEANLEEA3PROTOTYPE_API ACard : public AActor
{
	GENERATED_BODY()

	
public:	
	ACard();

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Card")
	UStaticMeshComponent* CardMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Card")
	UBoxComponent* CardHitBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Card")
	UWidgetComponent* LocationIndicationRef;

	UNiagaraComponent* TrailParticles;
	
	AActor* Parent;
	AActor* StuckActor;

	
public:	
	void ConsumeCard();
	void FreezeCard();

	virtual void Destroyed() override;


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCardOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnStuckActorDestroyed(AActor* DestroyedActor);
};
