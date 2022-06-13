// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "GameOverUI.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API UGameOverUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameOverUI")
	FName MainMenuName;

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* RetryButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuButton;


protected:
	void NativeConstruct() override;
	
	UFUNCTION()
	void RetryLevel();
	UFUNCTION()
	void ReturnToMainMenu();
};
