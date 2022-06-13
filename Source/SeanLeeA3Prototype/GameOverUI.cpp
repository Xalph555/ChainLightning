// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverUI.h"


void UGameOverUI::NativeConstruct()
{
	Super::NativeConstruct();

	RetryButton->OnReleased.AddDynamic(this, &UGameOverUI::RetryLevel);
	MainMenuButton->OnReleased.AddDynamic(this, &UGameOverUI::ReturnToMainMenu);
}


void UGameOverUI::RetryLevel()
{
	//UGameplayStatics::GetGameMode(GetWorld())->ResetLevel();
	UGameplayStatics::OpenLevel(GetWorld(), FName(UGameplayStatics::GetCurrentLevelName(GetWorld())));
	
	FInputModeGameOnly InputMode;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
}


void UGameOverUI::ReturnToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuName);

	FInputModeUIOnly InputMode;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
}
