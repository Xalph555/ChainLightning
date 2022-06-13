// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCharacter.h"
#include "Components/Textblock.h"
#include "CardCounterWidget.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API UCardCounterWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentCardCountLabel;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxCardCountLabel;

	
protected:
	void NativeConstruct() override;

	UFUNCTION()
	void OnCardCounterChanged(int CurrentCardCunt, int MaxCardCount);
};