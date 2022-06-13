// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);

	
	// properties
	SwitchRef = nullptr;

	EndLocationSetter = FVector(0.0f,0.0f,0.0f);
	EndLocation = FVector(0.0f,0.0f,0.0f);
	
	TravelTime = 5.0f;

	bIsOpening = false;
}


// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	// set EndLocation in world space
	FVector Direction = EndLocationSetter;
	Direction.Normalize();

	FRotator Rotation = GetActorRotation();
	Direction = Rotation.RotateVector(Direction);

	float Distance = FVector::Distance(FVector::ZeroVector, EndLocationSetter);
	
	EndLocation = GetActorLocation() + Direction * Distance;
	
	if (SwitchRef)
		SwitchRef->TriggeredDelegate.AddDynamic(this, &ADoor::OnSwitchTriggered);
}


// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// slide door to EndLocation
	if (bIsOpening)
	{
		//CurrentLerpTime = FMath::Min(1.0f, (CurrentLerpTime + DeltaTime) / TravelTime);
		SetActorLocation(UKismetMathLibrary::VInterpTo(GetActorLocation(), EndLocation, DeltaTime, TravelTime));
		
		if (GetActorLocation().Equals(EndLocation))
		{
			bIsOpening = false;
		}
	}
}


void ADoor::OnSwitchTriggered()
{
	bIsOpening = true;
	SwitchRef->TriggeredDelegate.RemoveDynamic(this, &ADoor::OnSwitchTriggered);

	// play activation sound
	if (SBOpen)
	{
		UGameplayStatics::SpawnSoundAttached(SBOpen, RootComponent);
	}
}

