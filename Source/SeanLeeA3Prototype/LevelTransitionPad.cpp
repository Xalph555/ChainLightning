// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransitionPad.h"
#include "PlayerCharacter.h"


// Sets default values
ALevelTransitionPad::ALevelTransitionPad()
{	
	TransitionHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TransitionHitBox"));
	RootComponent = TransitionHitBox;
}


// Called when the game starts or when spawned
void ALevelTransitionPad::BeginPlay()
{
	Super::BeginPlay();

	TransitionHitBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransitionPad::OnPadOverlapBegin);
}


void ALevelTransitionPad::OnPadOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Level Transition Pad Overlapped triggered"));
	
	if (OtherActor && OtherActor != this)
	{
		if (Cast<APlayerCharacter>(OtherActor) == Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()))
		{
			UGameplayStatics::OpenLevel(GetWorld(), TargetLevel);
		}
	}
}
