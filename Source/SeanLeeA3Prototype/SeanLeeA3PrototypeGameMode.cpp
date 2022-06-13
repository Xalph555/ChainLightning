// Copyright Epic Games, Inc. All Rights Reserved.

#include "SeanLeeA3PrototypeGameMode.h"

#include "PlayerCharacterController.h"
#include "SeanLeeA3PrototypeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASeanLeeA3PrototypeGameMode::ASeanLeeA3PrototypeGameMode()
{
	// Default player controller
	PlayerControllerClass = APlayerCharacterController::StaticClass();

	// default pawn
	static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerCharacterBP(TEXT("Blueprint'/Game/Blueprints/Player/BP_PlayerCharacter.BP_PlayerCharacter'"));
	
	if (PlayerCharacterBP.Object)
	{
		DefaultPawnClass = (UClass*)PlayerCharacterBP.Object->GeneratedClass;
	}
}
