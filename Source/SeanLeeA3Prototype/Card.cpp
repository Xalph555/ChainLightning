// Fill out your copyright notice in the Description page of Project Settings.


#include "Card.h"
#include "Card.h"


// Sets default values
ACard::ACard()
{
	// card mesh
	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CardMesh"));
	CardMesh->SetRelativeLocation(FVector(-12.5, -1.0, -16.0));
	CardMesh->SetRelativeScale3D(FVector(0.25, 0.01, 0.4));
	
	// Players can't walk on Card - adapted from First-Person Shooter Base Game PrototypeProjectile
	CardMesh->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CardMesh->CanCharacterStepUpOn = ECB_No;

	CardMesh->SetSimulatePhysics(true);
	CardMesh->SetCollisionObjectType(ECC_Projectile);
	CardMesh->SetCollisionResponseToChannel(ECC_Projectile, ECollisionResponse::ECR_Ignore);
	CardMesh->SetCollisionResponseToChannel(ECC_Player, ECollisionResponse::ECR_Ignore);
	
	RootComponent = CardMesh;

	// card hitbox
	CardHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CardHitBox"));
	CardHitBox->SetRelativeLocation(FVector(50.0, 0.0, 50.0));
	CardHitBox->SetRelativeScale3D(FVector(2.0, 11.0, 1.9));

	// Players can't walk on Card - adapted from First-Person Shooter Base Game PrototypeProjectile
	CardHitBox->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CardHitBox->CanCharacterStepUpOn = ECB_No;

	CardHitBox->SetCollisionObjectType(ECC_Projectile);
	CardHitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CardHitBox->SetCollisionResponseToChannel(ECC_Projectile, ECollisionResponse::ECR_Ignore);
	
	CardHitBox->SetupAttachment(CardMesh);

	// card indicator
	LocationIndicationRef = CreateDefaultSubobject<UWidgetComponent>(TEXT("LocationIndicator"));
	LocationIndicationRef->SetupAttachment(CardMesh);
	
	TrailParticles = nullptr;
	
	Parent = nullptr;
	StuckActor = nullptr;
}


// Called when the game starts or when spawned
void ACard::BeginPlay()
{
	Super::BeginPlay();
	
	CardHitBox->OnComponentBeginOverlap.AddDynamic(this, &ACard::OnCardOverlapBegin);

	// get reference to particle component
	TArray<UActorComponent*> ParticleEffects = GetComponentsByTag(UNiagaraComponent::StaticClass(), FName("ProjectileTrail"));

	if (ParticleEffects.Num() > 0)
	{
		TrailParticles = Cast<UNiagaraComponent>(ParticleEffects[0]);

		if (TrailParticles)
		{
			UE_LOG(LogTemp, Warning, TEXT("Card (%s) found its trail particles"), *GetActorLabel());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Card (%s) could not find its trail particles"), *GetActorLabel());
		}
	}
}


void ACard::ConsumeCard()
{
	if (StuckActor)
		StuckActor->OnDestroyed.RemoveDynamic(this, &ACard::OnStuckActorDestroyed);

	Destroy();
}


void ACard::FreezeCard()
{
	CardMesh->SetAllPhysicsLinearVelocity(FVector(0.0,0.0,0.0));
	CardMesh->SetSimulatePhysics(false);

	if (StuckActor)
	{
		StuckActor->OnDestroyed.RemoveDynamic(this, &ACard::OnStuckActorDestroyed);
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		
		StuckActor = nullptr;
	}

	if (TrailParticles)
	{
		TrailParticles->Deactivate();
	}
}

void ACard::Destroyed()
{
	Super::Destroyed();

	Parent = nullptr;
	StuckActor = nullptr;
}


void ACard::OnCardOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1,3.f, FColor::Yellow, TEXT("Card overlap event triggered"));

	if (!StuckActor && OtherActor && OtherActor != this)
	{
		if (!Parent)
			return;
		
		if (Parent == OtherActor)
			return;

		// check if hit actor has NonAttachable Interface
		if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UNonAttachable::StaticClass()))
		{
			INonAttachable::Execute_HitNonAttachable(OtherActor, CardMesh);
		}
		else
		{
			FreezeCard();

			StuckActor = OtherActor;
			StuckActor->OnDestroyed.AddDynamic(this, &ACard::OnStuckActorDestroyed);
		
			AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
}


void ACard::OnStuckActorDestroyed(AActor* DestroyedActor)
{
	if (DestroyedActor == StuckActor)
	{
		StuckActor->OnDestroyed.RemoveDynamic(this, &ACard::OnStuckActorDestroyed);
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		
		StuckActor = nullptr;
		
		CardMesh->SetSimulatePhysics(true);
	}
}

