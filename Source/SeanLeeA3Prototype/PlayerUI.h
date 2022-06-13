// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseUI.h"
#include "GameOverUI.h"
#include "PlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPlayerUI(const FObjectInitializer& ObjectInitializer);
	

protected:
	TSubclassOf<class UUserWidget> PauseUIRef;
	UPauseUI* PauseUIInstance;

	TSubclassOf<class UUserWidget> GameOverUIRef;
	UGameOverUI* GameOverUIInstance;

	
protected:
	void NativeConstruct() override;

	UFUNCTION()
	void OnGamePaused(bool IsPaused);
	UFUNCTION()
	void OnGameOver();
};
