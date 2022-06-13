// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsScreenUI.h"


void UOptionsScreenUI::NativeConstruct()
{
	Super::NativeConstruct();

	Parent = nullptr;

	MusicVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionsScreenUI::SetMusicVolume);
	SFXVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionsScreenUI::SetSFXVolume);
	ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UOptionsScreenUI::SetScreenResolution);
	MouseSensitivitySlider->OnValueChanged.AddDynamic(this, &UOptionsScreenUI::SetMouseSensitivity);
	
	BackButton->OnReleased.AddDynamic(this, &UOptionsScreenUI::ReturnToMainMenu);
}


void UOptionsScreenUI::SetMusicVolume(float Value)
{
	// to be added...
}


void UOptionsScreenUI::SetSFXVolume(float Value)
{
	// to be added...
}


void UOptionsScreenUI::SetScreenResolution(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	// to be added...
}


void UOptionsScreenUI::SetMouseSensitivity(float Value)
{
	// to be added...
}


void UOptionsScreenUI::ReturnToMainMenu()
{
	if (!Parent)
		return;

	Parent->SetTitleScreenVisibility(true);
	Parent->SetOptionsScreenVisibility(false);
}

