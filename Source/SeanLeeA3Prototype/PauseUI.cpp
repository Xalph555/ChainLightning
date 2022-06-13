// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseUI.h"
#include "LevelGameMode.h"
#include "PlayerCharacterController.h"


void UPauseUI::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton->OnReleased.AddDynamic(this, &UPauseUI::ResumeGame);
	MainMenuButton->OnReleased.AddDynamic(this, &UPauseUI::ReturnToMainMenu);
}


void UPauseUI::ResumeGame()
{
	ALevelGameMode* LevelGameMode = Cast<ALevelGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (LevelGameMode)
	{
		LevelGameMode->SetGamePause(false);
	}
}


void UPauseUI::ReturnToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuName);

	FInputModeUIOnly InputMode;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
}
