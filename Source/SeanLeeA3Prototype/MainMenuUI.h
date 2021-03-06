// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUI.generated.h"


/**
 * 
 */
UCLASS(Abstract)
class SEANLEEA3PROTOTYPE_API UMainMenuUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UTitleScreenUI* TitleScreen;
	UPROPERTY(meta = (BindWidget))
	class UOptionsScreenUI* OptionScreen;

	
public:
	void SetTitleScreenVisibility(bool Visible);
	void SetOptionsScreenVisibility(bool Visible);

	
protected:
	void NativeConstruct() override;
};