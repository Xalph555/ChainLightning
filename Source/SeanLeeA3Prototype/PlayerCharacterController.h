// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "PlayerUI.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerCharacterController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> PlayerUIRef;

	
protected:
	UPlayerUI* PlayerUIInstance;
	
	APlayerCharacter* PossessedPlayer;


public:
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	// HUD
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacterController")
	void EnableHUD(bool IsEnabled);

	// Enable/ disable controls
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacterController")
	void EnableControls(bool IsEnabled);

	// Movement
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void SprintStart();
	void SprintEnd();
	
	void JumpStart();
	void JumpEnd();

	// Mouse
	void MouseX(float AxisValue);
	void MouseY(float AxisValue);

	// card inputs
	void ThrowCard();
	void ActivateChainLightning();
	void ChangeConstruct();
	void ActivateLightningConstruct();
	void LightningConstructBreak();

	// pause game
	void PauseGame();

	
protected:
	virtual void BeginPlay() override;
};
