// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "SeanLeeA3Prototype.h"
#include "Damageable.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EnemyProjectile.generated.h"



UCLASS()
class SEANLEEA3PROTOTYPE_API AEnemyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyProjectile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EnemyProjectile")
	UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EnemyProjectile")
	UCapsuleComponent* ProjectileHitBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EnemyProjectile")
	float Damage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EnemyProjectile")
	float KnockBack;

	AActor* Parent;
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetParent(AActor* ParentActor);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnProjectileOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnParentDestroyed(AActor* DestroyedActor);
};
