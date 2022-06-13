// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Textblock.h"
#include "ManaBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API UManaBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ManaBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentManaLabel;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxManaLabel;

	
protected:
	void NativeConstruct() override;

	UFUNCTION()
	void OnManaChanged(float CurrentMana, float MaxMana);
};
