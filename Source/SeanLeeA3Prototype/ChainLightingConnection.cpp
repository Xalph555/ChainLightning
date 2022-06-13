// Fill out your copyright notice in the Description page of Project Settings.


#include "ChainLightingConnection.h"


// Sets default values
AChainLightingConnection::AChainLightingConnection()
{
	LightningMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightningMesh"));
	RootComponent = LightningMesh;
	
	Parent = nullptr;
}


void AChainLightingConnection::OnLifeTimeTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(LifeTimeTimerHandle);
	
	Destroy();
}


void AChainLightingConnection::Destroyed()
{
	Super::Destroyed();

	Parent = nullptr;
}
