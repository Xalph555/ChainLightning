// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "PauseUI.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API UPauseUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PauseUI")
	FName MainMenuName;

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* ResumeButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuButton;


protected:
	void NativeConstruct() override;
	
	UFUNCTION()
	void ResumeGame();
	UFUNCTION()
	void ReturnToMainMenu();
};
