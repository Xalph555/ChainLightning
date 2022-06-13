// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleScreenUI.h"



void UTitleScreenUI::NativeConstruct()
{
	Super::NativeConstruct();

	Parent = nullptr;

	PlayButton->OnReleased.AddDynamic(this, &UTitleScreenUI::StartGame);
	OptionsButton->OnReleased.AddDynamic(this, &UTitleScreenUI::OptionsMenu);
	QuitButton->OnReleased.AddDynamic(this, &UTitleScreenUI::QuitGame);
}


void UTitleScreenUI::StartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), StartingLevel);

	FInputModeGameOnly InputMode;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
}


void UTitleScreenUI::OptionsMenu()
{
	if (!Parent)
		return;

	Parent->SetTitleScreenVisibility(false);
	Parent->SetOptionsScreenVisibility(true);
}


void UTitleScreenUI::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}
