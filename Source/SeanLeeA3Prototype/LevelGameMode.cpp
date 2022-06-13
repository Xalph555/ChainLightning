// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGameMode.h"
#include "PlayerCharacterController.h"


ALevelGameMode::ALevelGameMode()
{
	// default player controller
	static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerCharacterControllerBP(TEXT("Blueprint'/Game/Blueprints/Player/BP_PlayerCharacterController.BP_PlayerCharacterController'"));
	
	if (PlayerCharacterControllerBP.Object)
	{
		PlayerControllerClass = (UClass*)PlayerCharacterControllerBP.Object->GeneratedClass;
	}
	
	
	// default pawn
	static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerCharacterBP(TEXT("Blueprint'/Game/Blueprints/Player/BP_PlayerCharacter.BP_PlayerCharacter'"));
	
	if (PlayerCharacterBP.Object)
	{
		DefaultPawnClass = (UClass*)PlayerCharacterBP.Object->GeneratedClass;
	}
}


void ALevelGameMode::SetGamePause(bool Paused) const
{
	UGameplayStatics::SetGamePaused(GetWorld(), Paused);
	GamePausedDelegate.Broadcast(Paused);
}


void ALevelGameMode::GameOver() const
{
	GameOverDelegate.Broadcast();
}
