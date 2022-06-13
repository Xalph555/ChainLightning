// Fill out your copyright notice in the Description page of Project Settings.


#include "Shield.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AShield::AShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	
	ShieldMesh->SetCollisionResponseToChannel(ECC_EnemyProjectile, ECollisionResponse::ECR_Ignore);
	ShieldMesh->SetCollisionResponseToChannel(ECC_Player, ECollisionResponse::ECR_Ignore);
	ShieldMesh->SetCollisionResponseToChannel(ECC_NonConductive, ECollisionResponse::ECR_Overlap);
	
	RootComponent = ShieldMesh;

	ShieldHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldHitBox"));
	
	ShieldHitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	ShieldHitBox->SetCollisionResponseToChannel(ECC_EnemyProjectile, ECollisionResponse::ECR_Ignore);
	ShieldMesh->SetCollisionResponseToChannel(ECC_NonConductive, ECollisionResponse::ECR_Overlap);
	
	ShieldHitBox->SetupAttachment(ShieldMesh);

	// properties
	bIsActive = true;
	Parent = nullptr;
}


void AShield::PlaySound(USoundBase* Sound)
{
	if (Sound)
	{
		UGameplayStatics::SpawnSoundAttached(Sound, RootComponent);
	}
}


void AShield::SetShieldActive(bool Active)
{
	bIsActive = Active;

	SetActorHiddenInGame(!Active);

	if (Active)
	{
		ShieldMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ShieldHitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		// play activate sound
		PlaySound(SBActivate);
	}
	else
	{
		ShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ShieldHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// play activate sound
		PlaySound(SBDeactivate);
	}

	UE_LOG(LogTemp, Warning, TEXT("Shield (%s) active: %d"), *GetActorLabel(), bIsActive);	
}


void AShield::HitNonAttachable_Implementation(UStaticMeshComponent* CollidingObject)
{
	CollidingObject->SetAllPhysicsLinearVelocity(FVector::Zero());

	// play activate sound
	PlaySound(SBHit);
}
