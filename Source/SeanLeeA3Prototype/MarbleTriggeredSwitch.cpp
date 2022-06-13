// Fill out your copyright notice in the Description page of Project Settings.


#include "MarbleTriggeredSwitch.h"

#include "Kismet/GameplayStatics.h"


AMarbleTriggeredSwitch::AMarbleTriggeredSwitch()
{
	ActivationInterval = 1.0f;
	
	TriggeringMarble = nullptr;
}


void AMarbleTriggeredSwitch::BeginPlay()
{
	Super::BeginPlay();

	SwitchHitBox->OnComponentBeginOverlap.AddDynamic(this, &AMarbleTriggeredSwitch::OnSwitchOverlapBegin);
	SwitchHitBox->OnComponentEndOverlap.AddDynamic(this, &AMarbleTriggeredSwitch::OnSwitchOverlapEnd);
}


void AMarbleTriggeredSwitch::OnActivationIntervalTimer()
{
	// switch triggers after marble has been on top of it for the ActivationInterval
	GetWorld()->GetTimerManager().ClearTimer(ActivationIntervalTimerHandle);
	bIsTriggered = true;
	UpdateMaterial();

	// play activation sound
	if (SBActivate)
	{
		UGameplayStatics::SpawnSoundAttached(SBActivate, RootComponent);
	}

	// logging
	UE_LOG(LogTemp, Warning, TEXT("Marble Triggered Switch (%s) Triggered "),
		*GetActorLabel());

	
	if (bIsMainSwitch)
	{
		MainSwitchCheck();
	}
	else
	{
		TriggeredDelegate.Broadcast();
	}
}


void AMarbleTriggeredSwitch::OnSwitchOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Something has overlapped the switch"))
			
		if (TriggeringMarble)
			return;

		AMarble* OverlappedMarble = Cast<AMarble>(OtherActor);

		if (IsValid(OverlappedMarble))
		{
			TriggeringMarble = OverlappedMarble;
			
			GetWorld()->GetTimerManager().SetTimer(ActivationIntervalTimerHandle, this, &AMarbleTriggeredSwitch::OnActivationIntervalTimer, ActivationInterval, false);

			// logging
			UE_LOG(LogTemp, Warning, TEXT("Marble has entered Marble Triggered Switch (%s) - Tracking Marble (%s)"),
				*GetActorLabel(), *OverlappedMarble->GetName());
		}
	}
}


void AMarbleTriggeredSwitch::OnSwitchOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Something has stopped overlapping the switch"))
		
		if (!TriggeringMarble)
			return;

		AMarble* OverlappedMarble = Cast<AMarble>(OtherActor);

		if (OverlappedMarble == TriggeringMarble)
		{
			GetWorld()->GetTimerManager().ClearTimer(ActivationIntervalTimerHandle);
			bIsTriggered = false;
			UpdateMaterial();

			TriggeringMarble = nullptr;

			// logging
			UE_LOG(LogTemp, Warning, TEXT("Tracked marble (%s) has left Marble Triggered Switch (%s)"),
				*OverlappedMarble->GetName(), *GetActorLabel());
		}
	}
}
