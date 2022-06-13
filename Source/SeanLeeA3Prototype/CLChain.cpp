// Fill out your copyright notice in the Description page of Project Settings.


#include "CLChain.h"

#include "Kismet/GameplayStatics.h"


ACLChain::ACLChain()
{
}


void ACLChain::SetConnection(AActor* SetParent, float Damage, float DamageSizeR, float DamageSizeL, float LifeTime)
{
	Parent = SetParent;

	// Set scale and orientation of mesh
	LightningMesh->SetRelativeScale3D(FVector(DamageSizeR, DamageSizeR, ((DamageSizeL + 2 * DamageSizeR) / 2.0f)));
	LightningMesh->AddRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	
	GetWorld()->GetTimerManager().SetTimer(LifeTimeTimerHandle, this, &ACLChain::OnLifeTimeTimer, LifeTime, false);

	//DealDamage(Damage, DamageSizeR, DamageSizeL);

	DealDamage(Damage, DamageSizeR, DamageSizeL);

	// play activate sound
	if (SBActivate)
	{
		UGameplayStatics::SpawnSoundAttached(SBActivate, RootComponent);
	}

	// logging
	UE_LOG(LogTemp, Display, TEXT("Chain Lightning Chain Bursted | Name: %s"), *GetName());
}


void ACLChain::DealDamage(float Damage, float DamageSizeR, float DamageSizeL)
{
	// adapted from FIT2096 Week 6 Lab
	
	FVector Location = GetActorLocation();
	FQuat DamageOrientation = GetActorRotation().Quaternion();
	float DamageLength = (DamageSizeL + 2 * DamageSizeR) / 2.0f;
	FCollisionShape HitCapsule = FCollisionShape::MakeCapsule(DamageSizeR, DamageLength);

	// logging
	//UE_LOG(LogTemp, Warning, TEXT("Chain Lightning Object | Name: %s |Location %s | Rotation: %s"),
	//	*GetName(), *GetActorLocation().ToString(), *GetActorRotation().ToString());

	// debug - draw shape of damaging capsule
	//DrawDebugCapsule(GetWorld(), Location, DamageLength, DamageSizeR, DamageOrientation, FColor::Red, false, 2.0f, 0, 0);

	TArray<FHitResult> OutHits;
	
	if (GetWorld()->SweepMultiByChannel(OutHits, Location, Location, DamageOrientation, ECC_WorldStatic, HitCapsule))
	{
		UE_LOG(LogTemp, Display, TEXT("Chain Lightning Chain has hit something | OutHits Results: %d"), OutHits.Num());
		
		for(auto& Hit: OutHits)
		{
			if (UKismetSystemLibrary::DoesImplementInterface(Hit.GetActor(), UDamageable::StaticClass()))
			{
				// logging
				UE_LOG(LogTemp, Display, TEXT("Chain Lightning Chain has dealt damage | Damage: %f | Hit Actor: %s | Location: %s | Rotation: %s"),
					Damage, *Hit.GetActor()->GetActorLabel(), *Hit.GetActor()->GetActorLocation().ToString(), *Hit.GetActor()->GetActorRotation().ToString());
				
				// apply damage to all damagable actors
				IDamageable::Execute_ApplyDamage(Hit.GetActor(), Damage, 0, this, Parent);
			}
		}
	}
}


void ACLChain::OnLifeTimeTimer()
{
	Super::OnLifeTimeTimer();
}
