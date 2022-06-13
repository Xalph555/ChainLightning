// Fill out your copyright notice in the Description page of Project Settings.


#include "CLSingle.h"

#include "Kismet/GameplayStatics.h"


ACLSingle::ACLSingle()
{
}


void ACLSingle::SetBurst(AActor* SetParent, float Damage, float KnockBack, float DamageSizeR, float LifeTime)
{
	Parent = SetParent;

	// Set scale of mesh
	LightningMesh->SetRelativeScale3D(FVector(DamageSizeR, DamageSizeR, DamageSizeR));
	
	GetWorld()->GetTimerManager().SetTimer(LifeTimeTimerHandle, this, &ACLSingle::OnLifeTimeTimer, LifeTime, false);

	DealDamage(Damage, KnockBack, DamageSizeR);

	// play activate sound
	if (SBActivate)
	{
		UGameplayStatics::SpawnSoundAttached(SBActivate, RootComponent);
	}

	// logging 
	UE_LOG(LogTemp, Display, TEXT("Chain Lightning Single Bursted | Name: %s"), *GetName());
}


void ACLSingle::DealDamage(float Damage, float KnockBack, float DamageSizeR)
{
	// adapted from FIT2096 Week 6 Lab
	
	FVector Location = GetActorLocation();
	FCollisionShape HitSphere = FCollisionShape::MakeSphere(DamageSizeR);

	// debug - draw shape of damaging sphere
	//DrawDebugSphere(GetWorld(), Location, HitSphere.GetSphereRadius(), 50, FColor::Red, false, 2.0f, 0, 0);

	TArray<FHitResult> OutHits;
	
	if (GetWorld()->SweepMultiByChannel(OutHits, Location, Location, FQuat::Identity, ECC_WorldStatic, HitSphere))
	{
		UE_LOG(LogTemp, Display, TEXT("Chain Lightning Single has hit something | OutHits Results: %d"), OutHits.Num());
		
		for(auto& Hit: OutHits)
		{
			if (UKismetSystemLibrary::DoesImplementInterface(Hit.GetActor(), UDamageable::StaticClass()))
			{
				// logging
				UE_LOG(LogTemp, Display, TEXT("Chain Lightning Single has dealt damage | Damage: %f | Hit Actor: %s | Location: %s | Rotation: %s"),
					Damage, *Hit.GetActor()->GetActorLabel(), *Hit.GetActor()->GetActorLocation().ToString(), *Hit.GetActor()->GetActorRotation().ToString());
				
				// apply damage to all damagable actors
				IDamageable::Execute_ApplyDamage(Hit.GetActor(), Damage, KnockBack, this, Parent);
			}
		}
	}
}


void ACLSingle::OnLifeTimeTimer()
{
	Super::OnLifeTimeTimer();
}
