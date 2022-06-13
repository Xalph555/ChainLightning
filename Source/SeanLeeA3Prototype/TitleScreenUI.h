// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuUI.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TitleScreenUI.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SEANLEEA3PROTOTYPE_API UTitleScreenUI : public UUserWidget
{
	GENERATED_BODY()

	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TitleScreenUI")
	FName StartingLevel;

	UMainMenuUI* Parent;

	
protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* PlayButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* OptionsButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	
protected:
	void NativeConstruct() override;

	UFUNCTION()
	void StartGame();
	UFUNCTION()
	void OptionsMenu();
	UFUNCTION()
	void QuitGame();
};
