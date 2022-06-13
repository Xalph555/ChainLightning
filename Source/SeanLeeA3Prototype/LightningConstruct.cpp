// Fill out your copyright notice in the Description page of Project Settings.


#include "LightningConstruct.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ALightningConstruct::ALightningConstruct()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConstructMesh"));
	ConstructMesh->SetCollisionObjectType(ECC_LightningConstruct);
	RootComponent = ConstructMesh;

	MaxMaterialAggressiveness = 10.0f;
	
	MaxHealth = 60.0;
	CurrentHealth = MaxHealth;
	
	Parent = nullptr;

	MaxLifeTime = 1.0f;
}


void ALightningConstruct::BeginPlay()
{
	Super::BeginPlay();

	if (BaseMaterial)
	{
		ConstructMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		ConstructMesh->SetMaterial(0, ConstructMaterial);
	}
}


void ALightningConstruct::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// determine stability of construct
	float ConstructHealthStability = 1 - (CurrentHealth / MaxHealth);
	float ConstructTimeStability = 1 - (UKismetSystemLibrary::K2_GetTimerRemainingTimeHandle(GetWorld(), LifeTimeTimerHandle) / MaxLifeTime);

	// logging
	//UE_LOG(LogTemp, Warning, TEXT("LC Health Stability: %f"), ConstructHealthStability);
	//UE_LOG(LogTemp, Warning, TEXT("LC Time Stability: %f"), ConstructTimeStability);
	
	// set material stability parameters according to TimeStability
	if (ConstructTimeStability > ConstructHealthStability && ConstructTimeStability > 0.5)
	{
		ConstructMaterial->SetScalarParameterValue("Aggressiveness", MaxMaterialAggressiveness * ConstructTimeStability);
		ConstructMaterial->SetScalarParameterValue("PulseFrequency", (2.1 * ConstructTimeStability) - 0.8);
	}
	// set material stability parameters according to HealthStability
	else
	{
		ConstructMaterial->SetScalarParameterValue("Aggressiveness", MaxMaterialAggressiveness * ConstructHealthStability);
		ConstructMaterial->SetScalarParameterValue("PulseFrequency", (2.1 * ConstructHealthStability) - 0.8);
	}
}


void ALightningConstruct::PlaySound(USoundBase* Sound, bool FixedLocation)
{
	if (!Sound)
		return;

	if (FixedLocation)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Sound, GetActorLocation());
	}
	else
	{
		UGameplayStatics::SpawnSoundAttached(Sound, RootComponent);
	}
}


void ALightningConstruct::SetConnection(AActor* SetParent, float Length, float LifeTime)
{
	Parent = SetParent;
	MaxLifeTime = LifeTime;

	// set mesh length
	ConstructMesh->SetRelativeScale3D(FVector(Length,
												ConstructMesh->GetComponentScale().Y,
												ConstructMesh->GetComponentScale().Z));
	
	GetWorld()->GetTimerManager().SetTimer(LifeTimeTimerHandle, this, &ALightningConstruct::OnLifeTimeTimer, MaxLifeTime, false);

	// play generate sound
	PlaySound(SBGenerate);
}


void ALightningConstruct::BreakConstruct()
{
	GetWorld()->GetTimerManager().ClearTimer(LifeTimeTimerHandle);
	
	// play break sound
	PlaySound(SBDestroy, true);
	
	Destroy();
}


void ALightningConstruct::Destroyed()
{
	Super::Destroyed();

	Parent = nullptr;
}


void ALightningConstruct::OnLifeTimeTimer()
{
	BreakConstruct();
}


void ALightningConstruct::ApplyDamage_Implementation(float DamageAmount, float KnockBackAmount, AActor* DamageObject, AActor* DamageParent)
{
	if (DamageParent == Parent)
		return;

	// logging
	UE_LOG(LogTemp, Display, TEXT("Lightning Construct damaged | Damage: %f"), DamageAmount);
	
	CurrentHealth -= DamageAmount;

	// play hit sound
	PlaySound(SBHit);

	if (CurrentHealth <= 0)
		// play break sound
		PlaySound(SBDestroy, true);
	
		BreakConstruct();
}