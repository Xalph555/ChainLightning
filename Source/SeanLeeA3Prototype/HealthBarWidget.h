// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Textblock.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentHealthLabel;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxHealthLabel;

	
protected:
	void NativeConstruct() override;

	UFUNCTION()
	void OnHealthChanged(float CurrentHealth, float MaxHealth);
};
