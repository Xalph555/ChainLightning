// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuUI.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "OptionsScreenUI.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SEANLEEA3PROTOTYPE_API UOptionsScreenUI : public UUserWidget
{
	GENERATED_BODY()


public:
	UMainMenuUI* Parent;

	
protected:
	UPROPERTY(meta = (BindWidget))
	class USlider* MusicVolumeSlider;
	UPROPERTY(meta = (BindWidget))
	class USlider* SFXVolumeSlider;
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ResolutionComboBox;
	UPROPERTY(meta = (BindWidget))
	class USlider* MouseSensitivitySlider;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;

	
protected:
	void NativeConstruct() override;

	UFUNCTION()
	void SetMusicVolume(float Value);
	UFUNCTION()
	void SetSFXVolume(float Value);
	UFUNCTION()
	void SetScreenResolution(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void SetMouseSensitivity(float Value);

	UFUNCTION()
	void ReturnToMainMenu();
};
