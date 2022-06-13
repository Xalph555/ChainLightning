// Fill out your copyright notice in the Description page of Project Settings.


#include "LightningTriggeredSwitch.h"

#include "Kismet/GameplayStatics.h"


ALightningTriggeredSwitch::ALightningTriggeredSwitch()
{
	ActivationInterval = 0.3f;
}


void ALightningTriggeredSwitch::OnActivationIntervalTimer()
{
	// deactivate switch
	GetWorld()->GetTimerManager().ClearTimer(ActivationIntervalTimerHandle);
	bIsTriggered = false;
	UpdateMaterial();

	UE_LOG(LogTemp, Warning, TEXT("Lightning Triggered Switch (%s) has deactivate"),
		*GetActorLabel());
}


void ALightningTriggeredSwitch::ApplyDamage_Implementation(float DamageAmount, float KnockBackAmount,
	AActor* DamageObject, AActor* DamageParent)
{
	// logging
	UE_LOG(LogTemp, Warning, TEXT("Lightning Triggered Switch (%s) is being damaged"),
		*GetActorLabel());
	
	if (bIsTriggered)
		return;
	
	if (DamageObject->IsA(AChainLightingConnection::StaticClass()))
	{
		bIsTriggered = true;
		UpdateMaterial();
		
		// play activation sound
		if (SBActivate)
		{
			UGameplayStatics::SpawnSoundAttached(SBActivate, RootComponent);
		}

		if (bIsMainSwitch)
		{
			MainSwitchCheck();
		}
		else
		{
			TriggeredDelegate.Broadcast();
		}

		GetWorld()->GetTimerManager().SetTimer(ActivationIntervalTimerHandle, this, &ALightningTriggeredSwitch::OnActivationIntervalTimer, ActivationInterval, false);

		// logging
		UE_LOG(LogTemp, Warning, TEXT("Lightning Triggered Switch (%s) Triggered"),
			*GetActorLabel());
	}
}