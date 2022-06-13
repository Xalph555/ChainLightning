// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "LevelGameMode.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisablePlayer);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePaused, bool, Paused);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);

UCLASS()
class SEANLEEA3PROTOTYPE_API ALevelGameMode : public AGameModeBase
{
	GENERATED_BODY()

	
public:
	ALevelGameMode();

	FOnGamePaused GamePausedDelegate;
	FOnGameOver GameOverDelegate;

	
public:
	// gamemode handles level wide pausing and game over events
	void SetGamePause(bool Paused) const;
	void GameOver() const;
};
