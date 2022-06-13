// Fill out your copyright notice in the Description page of Project Settings.


#include "Switch.h"


// Sets default values
ASwitch::ASwitch()
{
	SwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchMesh"));
	RootComponent = SwitchMesh;
	
	SwitchHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SwitchHitBox"));
	SwitchHitBox->SetupAttachment(SwitchMesh);

	// properties
	ActivationInterval = 0.0f;
	bIsMainSwitch = false;
	
	bIsTriggered = false;
}


// Called when the game starts or when spawned
void ASwitch::BeginPlay()
{
	Super::BeginPlay();
	
	BindConnectedSwitches();

	if (BaseMaterial)
	{
		SwitchMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		SwitchMesh->SetMaterial(0, SwitchMaterial);
	}
}


void ASwitch::MainSwitchCheck()
{
	if (!bIsMainSwitch)
		return;

	// check if all connected switches are triggered
	for (auto& Switch : ConnectedSwitches)
	{
		if (!Switch->bIsTriggered)
			return;
	}

	// check if the main switch is triggered
	if (bIsTriggered)
	{
		TriggeredDelegate.Broadcast();

		// logging
		UE_LOG(LogTemp, Warning, TEXT("Main Triggered Switch (%s) Triggered - Connected doors should open"),
			*GetActorLabel());
	}
}


void ASwitch::UpdateMaterial()
{
	SwitchMaterial->SetScalarParameterValue("Activated", bIsTriggered);
}


void ASwitch::BindConnectedSwitches()
{
	if (!bIsMainSwitch)
		return;

	for (auto& Switch : ConnectedSwitches)
	{
		// there should only ever be one main switch and it should not be connected to another main switcj
		checkf(!Switch->bIsMainSwitch, TEXT("A main switch cannot be connected to another main switch (MarbleTriggeredSwitch)!"));
		
		Switch->TriggeredDelegate.AddDynamic(this, &ASwitch::OnConnectedSwitchTriggered);
	}
}


void ASwitch::OnActivationIntervalTimer()
{}


void ASwitch::OnConnectedSwitchTriggered()
{
	MainSwitchCheck();
}

