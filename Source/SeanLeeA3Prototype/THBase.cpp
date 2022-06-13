// Fill out your copyright notice in the Description page of Project Settings.


#include "THBase.h"


ATHBase::ATHBase()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	PillarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillarMesh"));
	PillarMesh->SetupAttachment(RootComponent);

	BossStartTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BossStartTrigger"));
	BossStartTrigger->SetupAttachment(RootComponent);
}


void ATHBase::BeginPlay()
{
	Super::BeginPlay();

	BossStartTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATHBase::OnBossStartOverlapBegin);
	
	// variables to find components
	TArray<UActorComponent*> TempComponents = GetComponentsByTag(UChildActorComponent::StaticClass(), FName("Shield"));
	UChildActorComponent* TempChildComponent;
	
	// get shield reference
	if (TempComponents.Num() > 0)
	{
		TempChildComponent = Cast<UChildActorComponent>(TempComponents[0]);
		
		if (TempChildComponent && Cast<AShield>(TempChildComponent->GetChildActor()))
		{
			Shield = Cast<AShield>(TempChildComponent->GetChildActor());
			Shield->Parent = this;

			ActivateShield(false);
			
			UE_LOG(LogTemp, Display, TEXT("TH Boss (%s) Shield: %s"), *GetActorLabel(), *Shield->GetActorLabel());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TH Boss (%s) could not find its Shield after casting"), *GetActorLabel());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TH Boss (%s) could not find its Shield at all"), *GetActorLabel());
	}

	// get cannon section reference
	TempComponents = GetComponentsByTag(UChildActorComponent::StaticClass(), FName("Cannon"));

	if (TempComponents.Num() > 0)
	{
		TempChildComponent = Cast<UChildActorComponent>(TempComponents[0]);

		if (TempChildComponent && Cast<ATHCannonSection>(TempChildComponent->GetChildActor()))
		{
			CannonSection = Cast<ATHCannonSection>(TempChildComponent->GetChildActor());
			UE_LOG(LogTemp, Display, TEXT("TH Boss (%s) CannonSection: %s"), *GetActorLabel(), *CannonSection->GetActorLabel());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TH Boss (%s) could not find its CannonSection after casting"), *GetActorLabel());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TH Boss (%s) could not find its CannonSection at all"), *GetActorLabel());
	}

	// get pillar section references
	TempComponents = GetComponentsByTag(UChildActorComponent::StaticClass(), FName("PillarSection"));

	if (TempComponents.Num() > 0)
	{
		for (auto& TempComponent: TempComponents)
		{
			TempChildComponent = Cast<UChildActorComponent>(TempComponent);

			if (TempChildComponent && Cast<ATHPillarSection>(TempChildComponent->GetChildActor()))
			{
				PillarSections.Add(Cast<ATHPillarSection>(TempChildComponent->GetChildActor()));
				UE_LOG(LogTemp, Display, TEXT("TH Boss (%s) PillarSection: %s"), *GetActorLabel(), *PillarSections[PillarSections.Num() - 1]->GetActorLabel());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("TH Boss (%s) could not find a PillarSection after casting"), *GetActorLabel());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TH Boss (%s) could not find any of its PillarSections at all"), *GetActorLabel());
	}
}


void ATHBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


void ATHBase::ActivateShield(bool Active)
{
	if (!Shield)
		return;

	if (!bIsActive)
		return;

	Shield->SetShieldActive(Active);
}


void ATHBase::OnBossStartOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (Cast<APlayerCharacter>(OtherActor))
		{
			OnBossStartDelegate.Broadcast(Cast<APlayerCharacter>(OtherActor));
			BossStartTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			// logging
			UE_LOG(LogTemp, Warning, TEXT("TH Boss Fight Started (%s)"), *GetActorLabel());
		}
	}
}
