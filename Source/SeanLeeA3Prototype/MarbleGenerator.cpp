// Fill out your copyright notice in the Description page of Project Settings.


#include "MarbleGenerator.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AMarbleGenerator::AMarbleGenerator()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	GeneratorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GeneratorMesh"));
	GeneratorMesh->SetupAttachment(RootComponent);
	
	SpawnedMarbleRef = nullptr;
}


// Called when the game starts or when spawned
void AMarbleGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnMarble();
}


void AMarbleGenerator::SpawnMarble()
{
	if (SpawnedMarbleRef)
		return;
	
	const FVector SpawnLocation = GetActorLocation();
	
	// spawn the marble
	SpawnedMarbleRef = GetWorld()->SpawnActor<AMarble>(MarbleRef, SpawnLocation, FRotator::ZeroRotator);
	SpawnedMarbleRef->DissolveDelegate.AddDynamic(this, &AMarbleGenerator::OnMarbleDissolved);

	if (SBSpawn)
	{
		UGameplayStatics::SpawnSoundAttached(SBSpawn, RootComponent);
	}
}


void AMarbleGenerator::OnMarbleDissolved()
{
	SpawnedMarbleRef->DissolveDelegate.RemoveDynamic(this, &AMarbleGenerator::OnMarbleDissolved);
	SpawnedMarbleRef = nullptr;

	// spawn new marble when previous has been destroyed
	SpawnMarble();
}