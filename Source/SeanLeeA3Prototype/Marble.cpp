// Fill out your copyright notice in the Description page of Project Settings.


#include "Marble.h"

#include "Kismet/KismetMathLibrary.h"


// Sets default values
AMarble::AMarble()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MarbleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MarbleMesh"));
	MarbleMesh->SetSimulatePhysics(true);
	
	RootComponent = MarbleMesh;
	
	MarbleLifeTime = 10.0f;

	DissolveTime = 2.0f;
	IsDissolving = false;
	CurrentDissolveAmount = 1.0f;
}


// Called when the game starts or when spawned
void AMarble::BeginPlay()
{
	Super::BeginPlay();

	if (BaseMaterial)
	{
		MarbleMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		MarbleMesh->SetMaterial(0, MarbleMaterial);
	}
	
	GetWorld()->GetTimerManager().SetTimer(LifeTimeTimerHandle, this, &AMarble::OnLifeTimeTimer, MarbleLifeTime, false);
}


void AMarble::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsDissolving)
	{
		CurrentDissolveAmount = UKismetMathLibrary::FInterpTo_Constant(CurrentDissolveAmount, 0, DeltaSeconds, DissolveTime * 0.3);
		MarbleMaterial->SetScalarParameterValue("DissolveAmount", CurrentDissolveAmount);
	}
}


void AMarble::OnLifeTimeTimer()
{
	GetWorld()->GetTimerManager().SetTimer(DissolveTimeHandle, this, &AMarble::OnDissolveTimerTimer, DissolveTime, false);
	IsDissolving = true;

	// play dissolve sound
	if (SBDissolve)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SBDissolve, GetActorLocation());	
	}
}


void AMarble::OnDissolveTimerTimer()
{
	DissolveDelegate.Broadcast();
	Destroy();
}
