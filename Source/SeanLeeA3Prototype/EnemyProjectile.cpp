// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyProjectile.h"


// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	
	ProjectileMesh->SetCollisionObjectType(ECC_EnemyProjectile);
	ProjectileMesh->SetCollisionResponseToChannel(ECC_EnemyProjectile, ECollisionResponse::ECR_Ignore);
	ProjectileMesh->SetCollisionResponseToChannel(ECC_Player, ECollisionResponse::ECR_Ignore);
	
	RootComponent = ProjectileMesh;

	ProjectileHitBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ProjectileHitBox"));

	ProjectileHitBox->SetCollisionObjectType(ECC_EnemyProjectile);
	ProjectileHitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	ProjectileHitBox->SetCollisionResponseToChannel(ECC_EnemyProjectile, ECollisionResponse::ECR_Ignore);

	ProjectileHitBox->SetGenerateOverlapEvents(false);
	
	ProjectileHitBox->SetupAttachment(ProjectileMesh);

	// properties
	Damage = 0.0;
	KnockBack = 0.0;

	Parent = nullptr;
}


// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileHitBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectile::OnProjectileOverlapBegin);
}


// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AEnemyProjectile::SetParent(AActor* ParentActor)
{
	Parent = ParentActor;
	Parent->OnDestroyed.AddDynamic(this, &AEnemyProjectile::OnParentDestroyed);

	if (!Parent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Projectile parent not successfully set"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Projectile Projectile Parent: %s"), *Parent->GetActorLabel());
	}
	
	ProjectileHitBox->SetGenerateOverlapEvents(true);
}


void AEnemyProjectile::OnProjectileOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (Parent == OtherActor)
			return;

		// logging
		UE_LOG(LogTemp, Warning, TEXT("Enemy Projectile (%s) collided with %s | Location: %s | Rotation: %s"),
			*GetActorLabel(), *OtherActor->GetActorLabel(), *GetActorLocation().ToString(), *GetActorRotation().ToString());
		
		if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UDamageable::StaticClass()))
		{
			// apply damage
			IDamageable::Execute_ApplyDamage(OtherActor, Damage, KnockBack, this, Parent);
		}

		Destroy();
	}
}


void AEnemyProjectile::OnParentDestroyed(AActor* DestroyedActor)
{
	Parent = nullptr;
}

