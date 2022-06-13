// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuUI.h"
#include "TitleScreenUI.h"
#include "OptionsScreenUI.h"


void UMainMenuUI::NativeConstruct()
{
	Super::NativeConstruct();

	TitleScreen->Parent = this;
	OptionScreen->Parent = this;

	SetTitleScreenVisibility(true);
	SetOptionsScreenVisibility(false);
}


void UMainMenuUI::SetTitleScreenVisibility(bool Visible)
{
	if (Visible)
	{
		TitleScreen->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		TitleScreen->SetVisibility(ESlateVisibility::Collapsed);
	}
}


void UMainMenuUI::SetOptionsScreenVisibility(bool Visible)
{
	if (Visible)
	{
		OptionScreen->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		OptionScreen->SetVisibility(ESlateVisibility::Collapsed);
	}
}

