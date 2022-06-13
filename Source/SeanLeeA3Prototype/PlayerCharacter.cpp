// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "LevelGameMode.h"
#include "PlayerCharacterController.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(50.f, 89.0f);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Player);

	// camera
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(GetCapsuleComponent());
	PlayerCamera->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	PlayerCamera->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	//FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->SetupAttachment(PlayerCamera);
	//FirstPersonMesh->bCastDynamicShadow = false;
	//FirstPersonMesh->CastShadow = false;
	//FirstPersonMesh->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	//FirstPersonMesh->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// mesh that is visible in the world
	VisibleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleMesh"));
	VisibleMesh->SetupAttachment(GetCapsuleComponent());

	VisibleMesh->SetOwnerNoSee(true);
	VisibleMesh->bCastDynamicShadow = true;
	VisibleMesh->CastShadow = true;

	bIsDead = false;
	
	// health
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	HealthRegenRate = 5.0f;
	HealthRegenTime = 1.0f;
	HealthRegenDelay = 5.0f;

	bCanHealthRegen = false;

	// mana
	MaxMana = 80.0f;
	CurrentMana = MaxMana;
	ManaRegenRate = 10.0f;
	ManaRegenTime = 1.0f;
	
	
	// card properties
	CardThrowPointOffset = FVector(1.0f, 0.0f, 50.0f);
	
	MaxCardCount = 5;
	CurrentCardCount = 0;

	CardThrowForce = 2000.0f;

	// chain lightning properties
	CLBaseManaCost = 8.0f;
	CLScaleManaCost = 6.0f;
	
	CLConnectionDist = 1500.0f;
	
	CLDamageLifeTime = 2.0f;

	// chain lightning properties - single
	CLSingleDamage = 15.0f;
	CLSingleRadius = 100.0f;
	CLSingleKnockBack = 800;

	// chain lightning properties - chain
	CLChainDamageMax = 60.0f;
	CLChainDamageMin = 18.0f;
	CLChainRadius = 60.0f;
	CLChainTraceRadiusThreshold = 0.6f;
	CLChainConnectionDistMin = 30.0f;

	// lightning construct properties
	LCBaseManaCost = 12.0f;
	LCScaleManaCost = 8.0f;
	LCConnectionDist = 1250.0f;
	LCLifeTime = 20.0f;
	bIsLCWall = true;

	//  player lightning circle particles
	LightningCircleParticles = nullptr;
	
	// movement speed properties
	WalkSpeed = 500.0f;
	SprintSpeed = 700.0f;
	bIsSprinting = false;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// jump properties
	JumpHeight = 500.0f;
	FallingGravityScale = 1.5f;
	
	GetCharacterMovement()->JumpZVelocity = JumpHeight;

	// Other character movement component settings
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->AirControlBoostVelocityThreshold = 0.0f;
	GetCharacterMovement()->FallingLateralFriction = 1.0f;
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// get reference to particle component
	TArray<UActorComponent*> ParticleEffects = GetComponentsByTag(UNiagaraComponent::StaticClass(), FName("LightningCircleParticles"));

	if (ParticleEffects.Num() > 0)
	{
		LightningCircleParticles = Cast<UNiagaraComponent>(ParticleEffects[0]);

		if (LightningCircleParticles)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player (%s) found its lightning circle particles"), *GetActorLabel());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Card (%s) could not find its lightning circle particles"), *GetActorLabel());
		}
	}
}


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// player falls faster than they jump
	if(GetCharacterMovement()->Velocity.Z < 0)
	{
		GetCharacterMovement()->GravityScale = FallingGravityScale;
	}
	else
	{
		GetCharacterMovement()->GravityScale = 1.0f;
	}

	// sprinting
	if (bIsSprinting && !GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

	// fix particle rotation
	// display player particles
	LightningCircleParticles->SetWorldRotation(FRotator::ZeroRotator);
	
	/*// logging
	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::White,
			FString::Printf(TEXT("Card Count: %d / %d"),
				CurrentCardCount, MaxCardCount));
	
	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::White,
			FString::Printf(TEXT("Mana: %f / %f"),
				CurrentMana, MaxMana));
	
	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::White,
			FString::Printf(TEXT("Health: %f / %f"),
				CurrentHealth, MaxHealth));*/
}

void APlayerCharacter::PlaySound(USoundBase* Sound)
{
	if (Sound)
	{
		UGameplayStatics::SpawnSoundAttached(Sound, RootComponent);
	}
}


void APlayerCharacter::ThrowCard()
{
	if (!CardProjectile)
		return;

	if (GetCurrentCardCount() >= GetMaxCardCount())
		return;
	
	//GEngine->AddOnScreenDebugMessage(-1,3.f, FColor::White, TEXT("Player has thrown card"));
	
	// adapted from First-Person Shooter Base Game TP_WeaponComponent Fire() function
	const APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(GetController());
	
	const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(CardThrowPointOffset);
	
	// spawn collision handling override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	// spawn the card
	ACard* SpawnedCard = GetWorld()->SpawnActor<ACard>(CardProjectile, SpawnLocation, SpawnRotation, ActorSpawnParams);

	if (SpawnedCard)
	{
		SpawnedCard->Parent = this;
		SpawnedCard->OnDestroyed.AddDynamic(this, &APlayerCharacter::OnCardDestroyed);

		// add to thrown card references
		ThrownCards.Add(SpawnedCard);
		AddCurrentCardCount(1);

		// add throw force
		FVector ThrowForce = SpawnLocation - (GetActorLocation() + SpawnRotation.RotateVector(FVector(0.0f, 0.0f, CardThrowPointOffset.Z)));
		ThrowForce.Normalize();
		ThrowForce *= CardThrowForce;
	
		SpawnedCard->CardMesh->AddVelocityChangeImpulseAtLocation(ThrowForce, SpawnLocation);

		// play hurt sound
		PlaySound(SBThrowCard);
	}
}


void APlayerCharacter::DestroyCards()
{
	for (auto& Card : ThrownCards)
	{
		Card->OnDestroyed.RemoveDynamic(this, &APlayerCharacter::OnCardDestroyed);
		Card->ConsumeCard();
	}
	
	ThrownCards.Empty();

	SetCurrentCardCount(0);
}


void APlayerCharacter::OnCardDestroyed(AActor* DestroyedActor)
{
	ACard* DestroyedCard = Cast<ACard>(DestroyedActor);
	
	if (DestroyedCard)
	{
		// remove destroyed card
		const int32 CardIndex = ThrownCards.IndexOfByKey(DestroyedCard);

		if (CardIndex != INDEX_NONE)
		{
			ThrownCards[CardIndex]->OnDestroyed.RemoveDynamic(this, &APlayerCharacter::OnCardDestroyed);
			
			ThrownCards.RemoveAtSwap(CardIndex, 1);
			AddCurrentCardCount(-1);
		}	
	}
}


void APlayerCharacter::ChainLightning()
{
	if (!ChainLightningSingle || !ChainLightningChain)
		return;

	if (ThrownCards.Num() <= 0)
		return;
	
	// calculate mana cost
	float ManaCost = CLBaseManaCost + CLScaleManaCost * FMath::Max(ThrownCards.Num() - 1, 0);
	
	if (GetCurrentMana() < ManaCost)
		return;

	AddCurrentMana(-ManaCost);
	
	// logging
	UE_LOG(LogTemp, Warning, TEXT("Chain Lightning Activated"));
	//GEngine->AddOnScreenDebugMessage(-1,3.f, FColor::White, TEXT("Player activated Chain Lightning"));
	
	UE_LOG(LogTemp, Warning, TEXT("Number of Thrown Cards: %d"), ThrownCards.Num());
	
	// freeze position of all thrown cards
	for (auto& card : ThrownCards)
	{
		card->FreezeCard();
	}

	bool HasChained = false;
	
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	// determine whether to do single burst or chain
	for (int i = 0; i < ThrownCards.Num() - 1; i++)
	{
		FVector StartPoint = ThrownCards[i]->GetActorLocation();
		FVector EndPoint = ThrownCards[i+1]->GetActorLocation();
		float PointDistance = FVector::Distance(StartPoint, EndPoint);
		
		if (PointDistance <= CLConnectionDist && !IsNonConductiveObjectInPath(StartPoint, EndPoint))
		{
			// calculate damage
			float Damage = CLChainDamageMax * (1 - (FMath::Max(PointDistance - CLChainConnectionDistMin, 0.1)/ CLConnectionDist));

			Damage = FMath::Clamp(Damage, CLChainDamageMin, CLChainDamageMax);

			// calculate orientation
			FVector Direction = EndPoint - StartPoint;
			Direction.Normalize();

			FVector MidPoint = FVector((StartPoint.X + EndPoint.X) / 2.0f,
								(StartPoint.Y + EndPoint.Y) / 2.0f,
								(StartPoint.Z + EndPoint.Z) / 2.0f);
			
			FRotator ChainRotation = Direction.Rotation();
			
			// spawn CL Chain
			ACLChain* CLChain = GetWorld()->SpawnActor<ACLChain>(ChainLightningChain, MidPoint, ChainRotation, ActorSpawnParams);
			//CLChain->SetConnection(this, Damage, CLChainRadius, PointDistance, CLDamageLifeTime);
			CLChain->SetConnection(this, Damage, CLChainRadius, PointDistance, CLDamageLifeTime);

			HasChained = true;

			// logging
			UE_LOG(LogTemp, Warning, TEXT("Chain Lightning Chained | Name: %s |Location %s | Rotation: %s"),
				*CLChain->GetName(), *CLChain->GetActorLocation().ToString(), *CLChain->GetActorRotation().ToString());
		}
		else
		{
			if (HasChained)
			{
				HasChained = false;
				continue;
			}
				
			ACLSingle* CLBurst = GetWorld()->SpawnActor<ACLSingle>(ChainLightningSingle, StartPoint, FRotator::ZeroRotator, ActorSpawnParams);
			CLBurst->SetBurst(this, CLSingleDamage, CLSingleKnockBack, CLSingleRadius, CLDamageLifeTime);

			// logging
			UE_LOG(LogTemp, Warning, TEXT("Chain Lightning Single | Name: %s |Location %s | Rotation: %s"),
				*CLBurst->GetName(), *CLBurst->GetActorLocation().ToString(), *CLBurst->GetActorRotation().ToString());
		}
	}

	// determine what to do with final card
	if (!HasChained)
	{
		ACLSingle* CLBurst = GetWorld()->SpawnActor<ACLSingle>(ChainLightningSingle, ThrownCards[ThrownCards.Num() - 1]->GetActorLocation(), FRotator::ZeroRotator, ActorSpawnParams);
		CLBurst->SetBurst(this, CLSingleDamage, CLSingleKnockBack, CLSingleRadius, CLDamageLifeTime);

		// logging
		UE_LOG(LogTemp, Warning, TEXT("Chain Lightning Single Final"));
	}

	// logging
	UE_LOG(LogTemp, Warning, TEXT("Chain Lightning Ability Finished"));
	
	// remove all cards
	DestroyCards();

	// display player particles
	if (LightningCircleParticles)
	{
		LightningCircleParticles->DeactivateImmediate();
		LightningCircleParticles->Activate();	
	}

	// play hurt sound
	PlaySound(SBAbilityActivation);
}


bool APlayerCharacter::IsNonConductiveObjectInPath(FVector StartPoint, FVector EndPoint)
{
	// adapted from FIT2096 week 6 labs

	TArray<FHitResult> OutHits;

	// clear hit data
	for (auto& Hit: OutHits)
	{
		Hit = FHitResult(ForceInit);
	}

	TArray<AActor*> ActorsToIgnore;

	if (TraceForNonConductive(GetWorld(), ActorsToIgnore, StartPoint, EndPoint, OutHits, ECC_NonConductive))
	{
		UE_LOG(LogClass, Warning, TEXT("Chain Lightning found Non-Conductive object in chain path | StartPoint: %s | EndPoint: %s"),
			*StartPoint.ToString(), *EndPoint.ToString());
		
		return true;
	}

	return false;
}


bool APlayerCharacter::TraceForNonConductive(UWorld* World, TArray<AActor*>& ActorsToIgnore, const FVector& Start,
	const FVector& End, TArray<FHitResult>& OutHits, ECollisionChannel CollisionChannel)
{
	// adapted from FIT2096 week 6 labs

	if (!World)
		return false;

	// clear hit data
	for (auto& Hit: OutHits)
	{
		Hit = FHitResult(ForceInit);
	}

	float TraceRadius = CLChainRadius * CLChainTraceRadiusThreshold;

	// adjust start and end points to take into account radius of shape trace
	FVector DirToEnd = End - Start;
	DirToEnd.Normalize();

	FVector StartAdjusted = Start + DirToEnd * TraceRadius;
	FVector EndAdjusted = End - DirToEnd * TraceRadius;
	
	UKismetSystemLibrary::SphereTraceMulti(World, StartAdjusted, EndAdjusted, TraceRadius, UEngineTypes::ConvertToTraceType(CollisionChannel),
		false, ActorsToIgnore, EDrawDebugTrace::None, OutHits, true, FLinearColor::Blue, FLinearColor::Green, CLDamageLifeTime);

	return (OutHits.Num() > 0);
}


void APlayerCharacter::ChangeConstruct()
{
	bIsLCWall = !bIsLCWall;
	
	ConstructChangedDelegate.Broadcast(bIsLCWall);

	//GEngine->AddOnScreenDebugMessage(-1,3.f, FColor::White, TEXT("Player activated Lightning Construct"));
	
	/*GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White,
			FString::Printf(TEXT("Player changed construct | IsLCWall: %d"),
				bIsLCWall));*/
}


void APlayerCharacter::LightningConstruct()
{
	if (!LightningConstructWall || !LightningConstructBridge)
		return;

	if (ThrownCards.Num() <= 1)
		return;

	// calculate mana cost
	float ManaCost = LCBaseManaCost + LCScaleManaCost * FMath::Max(ThrownCards.Num() - 2, 0);
	
	if (GetCurrentMana() < ManaCost)
		return;

	AddCurrentMana(-ManaCost);

	// logging
	UE_LOG(LogTemp, Warning, TEXT("Lightning Construct Activated"));
	//GEngine->AddOnScreenDebugMessage(-1,3.f, FColor::White, TEXT("Player activated Lightning Construct"));
	
	UE_LOG(LogTemp, Warning, TEXT("Number of Thrown Cards: %d"), ThrownCards.Num());

	// freeze position of all thrown cards
	for (auto& card : ThrownCards)
	{
		card->FreezeCard();
	}

	TArray<std::pair<int, int>> ValidPairs;
	
	// count valid pairs
	for (int i = 0; i < ThrownCards.Num() - 1; i++)
	{
		if (FVector::Distance(ThrownCards[i]->GetActorLocation(), ThrownCards[i + 1]->GetActorLocation()) <= LCConnectionDist)
		{
			ValidPairs.Add(std::make_pair(i, i + 1));
		}
	} 
	
	// create constructs - even number of valid pairs - create between every pair
	if (ValidPairs.Num() % 2 == 0)
	{
		for (int i = 0; i < ValidPairs.Num(); i ++)
		{
			SpawnConstruct(ThrownCards[ValidPairs[i].first]->GetActorLocation(),
							ThrownCards[ValidPairs[i].second]->GetActorLocation());
		}
	}
	// create constructs - odd number of valid pairs - create between every second pair 
	else
	{
		for (int i = 0; i < ValidPairs.Num(); i += 2)
		{
			SpawnConstruct(ThrownCards[ValidPairs[i].first]->GetActorLocation(),
							ThrownCards[ValidPairs[i].second]->GetActorLocation());
		}
	}

	// logging
	UE_LOG(LogTemp, Warning, TEXT("Lighting Construct Ability Finished"));
	
	// remove all cards
	DestroyCards();

	// display player particles
	if (LightningCircleParticles)
	{
		LightningCircleParticles->DeactivateImmediate();
		LightningCircleParticles->Activate();
	}

	// play hurt sound
	PlaySound(SBAbilityActivation);
}


void APlayerCharacter::SpawnConstruct(FVector StartPoint, FVector EndPoint)
{
	float PointDistance = FVector::Distance(StartPoint, EndPoint);

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector Direction = EndPoint - StartPoint;
	Direction.Normalize();
			
	//FVector MidPoint = StartPoint + Direction * (PointDistance / 2.0f);
	FVector MidPoint = FVector((StartPoint.X + EndPoint.X) / 2.0f,
								(StartPoint.Y + EndPoint.Y) / 2.0f,
								(StartPoint.Z + EndPoint.Z) / 2.0f);
	
	FRotator ConstructRotation = Direction.Rotation();


	ALightningConstruct* SpawnedConstruct;

	// spawn wall
	if (bIsLCWall)
	{
		SpawnedConstruct =	GetWorld()->SpawnActor<ALightningConstruct>(LightningConstructWall, MidPoint, ConstructRotation, ActorSpawnParams);
	}
	// spawn bridge
	else
	{
		SpawnedConstruct =	GetWorld()->SpawnActor<ALightningConstruct>(LightningConstructBridge, MidPoint, ConstructRotation, ActorSpawnParams);
	}

	if (SpawnedConstruct)
	{
		SpawnedConstruct->OnDestroyed.AddDynamic(this, &APlayerCharacter::OnConstructDestroyed);
		SpawnedConstruct->SetConnection(this, PointDistance, LCLifeTime);

		GeneratedConstructs.Add(SpawnedConstruct);
	
		// logging
		UE_LOG(LogTemp, Warning, TEXT("Lightning Construct Spawned | Name: %s | Location %s | Rotation: %s"),
			*SpawnedConstruct->GetName(), *SpawnedConstruct->GetActorLocation().ToString(), *SpawnedConstruct->GetActorRotation().ToString());	
	}
}


void APlayerCharacter::DestroyConstructs()
{
	for (auto& Construct : GeneratedConstructs)
	{
		Construct->OnDestroyed.RemoveDynamic(this, &APlayerCharacter::OnConstructDestroyed);
		Construct->BreakConstruct();
	}

	GeneratedConstructs.Empty();
}


void APlayerCharacter::OnConstructDestroyed(AActor* DestroyedActor)
{
	ALightningConstruct* DestroyedConstruct = Cast<ALightningConstruct>(DestroyedActor);
	
	if (DestroyedConstruct)
	{
		// remove destroyed card
		const int32 ConstructIndex = GeneratedConstructs.IndexOfByKey(DestroyedConstruct);

		if (ConstructIndex != INDEX_NONE)
		{
			GeneratedConstructs[ConstructIndex]->OnDestroyed.RemoveDynamic(this, &APlayerCharacter::OnConstructDestroyed);
			
			GeneratedConstructs.RemoveAtSwap(ConstructIndex, 1);
		}	
	}
}


void APlayerCharacter::ApplyDamage_Implementation(float DamageAmount, float KnockBackAmount, AActor* DamageObject, AActor* DamageParent)
{
	if (DamageParent == this)
		return;

	// logging
	UE_LOG(LogTemp, Display, TEXT("Player damaged | Damage: %f"), DamageAmount);

	// applying knockback
	FVector Direction = GetActorLocation() - DamageObject->GetActorLocation();
	Direction.Normalize();

	FVector KnockBackForce = Direction * KnockBackAmount;
	
	LaunchCharacter(KnockBackForce, false, false);

	AddCurrentHealth(-DamageAmount);

	// play hurt sound
	PlaySound(SBHurt);

	// game over when out of health
	if (GetCurrentHealth() <= 0)
	{
		PlayerDeath();
	}
}


void APlayerCharacter::PlayerDeath()
{
	if (bIsDead)
		return;
	
	ALevelGameMode* LevelGameMode = Cast<ALevelGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (LevelGameMode)
	{
		LevelGameMode->GameOver();
			
		// play hurt sound
		PlaySound(SBDeath);

		bIsDead = true;
	}
}


void APlayerCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	PlayerDeath();
}


// timer callbacks
// -------------------------------------------------
void APlayerCharacter::OnHealthRegenTimer()
{
	if (bCanHealthRegen)
		AddCurrentHealth(HealthRegenRate);

	// stop regen if maxed
	if (GetCurrentHealth() >= GetMaxHealth())
		GetWorld()->GetTimerManager().ClearTimer(HealthRegenTimerHandle);
}


void APlayerCharacter::OnHealthDelayTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(HealthRegenDelayTimerHandle);

	bCanHealthRegen = true;
	GetWorld()->GetTimerManager().SetTimer(HealthRegenTimerHandle, this, &APlayerCharacter::OnHealthRegenTimer, HealthRegenTime, true, 0.0f);
}


void APlayerCharacter::OnManaRegenTimer()
{
	AddCurrentMana(ManaRegenRate);

	// stop regen if maxed
	if (GetCurrentMana() >= GetMaxMana())
		GetWorld()->GetTimerManager().ClearTimer(ManaRegenTimerHandle);
}


// setters and getters
// -------------------------------------------------
float APlayerCharacter::GetMaxHealth() const
{
	return MaxHealth;
}

void APlayerCharacter::SetMaxHealth(float Health)
{
	MaxHealth = Health;

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}

void APlayerCharacter::AddMaxHealth(float Health)
{
	MaxHealth += Health;

	if (MaxHealth < 0)
		MaxHealth = 0.0f;

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}


float APlayerCharacter::GetCurrentHealth() const
{
	return CurrentHealth;
}

void APlayerCharacter::SetCurrentHealth(float Health)
{
	// regen timer
	if (Health < CurrentHealth)
	{
		bCanHealthRegen = false;
		GetWorld()->GetTimerManager().ClearTimer(HealthRegenDelayTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(HealthRegenDelayTimerHandle, this, &APlayerCharacter::OnHealthDelayTimer, HealthRegenDelay, false);
	}
	
	CurrentHealth = Health;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}

void APlayerCharacter::AddCurrentHealth(float Health)
{
	CurrentHealth += Health;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);

	// regen timer
	if (Health < 0)
	{
		bCanHealthRegen = false;
		GetWorld()->GetTimerManager().ClearTimer(HealthRegenDelayTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(HealthRegenDelayTimerHandle, this, &APlayerCharacter::OnHealthDelayTimer, HealthRegenDelay, false);
	}

	HealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
}


float APlayerCharacter::GetMaxMana() const
{
	return MaxMana;
}

void APlayerCharacter::SetMaxMana(float Mana)
{
	MaxMana = Mana;
	
	ManaChangedDelegate.Broadcast(CurrentMana, MaxMana);
}

void APlayerCharacter::AddMaxMana(float Mana)
{
	MaxMana += Mana;

	if (MaxMana <= 0)
		MaxMana = 0.0f;

	ManaChangedDelegate.Broadcast(CurrentMana, MaxMana);
}


float APlayerCharacter::GetCurrentMana() const
{
	return CurrentMana;
}

void APlayerCharacter::SetCurrentMana(float Mana)
{
	CurrentMana = Mana;
	CurrentMana = FMath::Clamp(CurrentMana, 0, MaxMana);

	// start regen
	GetWorld()->GetTimerManager().SetTimer(ManaRegenTimerHandle, this, &APlayerCharacter::OnManaRegenTimer, ManaRegenTime, true);
	
	ManaChangedDelegate.Broadcast(CurrentMana, MaxMana);
}

void APlayerCharacter::AddCurrentMana(float Mana)
{
	CurrentMana += Mana;
	CurrentMana = FMath::Clamp(CurrentMana, 0, MaxMana);

	// start regen
	GetWorld()->GetTimerManager().SetTimer(ManaRegenTimerHandle, this, &APlayerCharacter::OnManaRegenTimer, ManaRegenTime, true);

	ManaChangedDelegate.Broadcast(CurrentMana, MaxMana);
}


int APlayerCharacter::GetMaxCardCount() const
{
	return MaxCardCount;
}

void APlayerCharacter::SetMaxCardCount(int Cards)
{
	MaxCardCount = Cards;

	if (MaxCardCount < 0)
		MaxCardCount = 0;

	CardCountChangedDelegate.Broadcast(CurrentCardCount, MaxCardCount);
}

void APlayerCharacter::AddMaxCardCount(int Cards)
{
	MaxCardCount += Cards;

	if (MaxCardCount < 0)
		MaxCardCount = 0;

	CardCountChangedDelegate.Broadcast(CurrentCardCount, MaxCardCount);
}


int APlayerCharacter::GetCurrentCardCount() const
{
	return CurrentCardCount;
}

void APlayerCharacter::SetCurrentCardCount(int Cards)
{
	CurrentCardCount = Cards;
	CurrentCardCount = FMath::Clamp(CurrentCardCount, 0, MaxCardCount);

	CardCountChangedDelegate.Broadcast(CurrentCardCount, MaxCardCount);
}

void APlayerCharacter::AddCurrentCardCount(int Cards)
{
	CurrentCardCount += Cards;
	CurrentCardCount = FMath::Clamp(CurrentCardCount, 0, MaxCardCount);

	CardCountChangedDelegate.Broadcast(CurrentCardCount, MaxCardCount);
}