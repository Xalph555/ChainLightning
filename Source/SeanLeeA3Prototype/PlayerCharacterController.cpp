// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "LevelGameMode.h"



APlayerCharacterController::APlayerCharacterController()
{
	PlayerUIInstance = nullptr;
}


void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	// add PlayerUI to viewport
	if (PlayerUIRef)
	{
		PlayerUIInstance = CreateWidget<UPlayerUI>(this, PlayerUIRef);
		PlayerUIInstance->AddToViewport(0);
	}
	else
	{
		// logging
		UE_LOG(LogTemp, Warning, TEXT("UI failed to load on APlayerCharacterController (%s)"), *GetActorLabel());
	}
}


void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Movement
	InputComponent->BindAxis("MoveForward", this, &APlayerCharacterController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacterController::MoveRight);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &APlayerCharacterController::SprintStart);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &APlayerCharacterController::SprintEnd);
	
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &APlayerCharacterController::JumpStart);
	InputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &APlayerCharacterController::JumpEnd);
	
	// Mouse
	InputComponent->BindAxis("MouseX", this, &APlayerCharacterController::MouseX);
	InputComponent->BindAxis("MouseY", this, &APlayerCharacterController::MouseY);

	// Card
	InputComponent->BindAction("ThrowCard", EInputEvent::IE_Released, this, &APlayerCharacterController::ThrowCard);

	// Lightning Abilities
	InputComponent->BindAction("ChanLightning", EInputEvent::IE_Released, this, &APlayerCharacterController::ActivateChainLightning);
	InputComponent->BindAction("ConstructCycle", EInputEvent::IE_Released, this, &APlayerCharacterController::ChangeConstruct);
	InputComponent->BindAction("LightningConstruct", EInputEvent::IE_Released, this, &APlayerCharacterController::ActivateLightningConstruct);
	InputComponent->BindAction("LightningConstructBreak", EInputEvent::IE_Released, this, &APlayerCharacterController::LightningConstructBreak);

	// Pause Game - can execute when game is paused
	InputComponent->BindAction("Pause", EInputEvent::IE_Released, this, &APlayerCharacterController::PauseGame).bExecuteWhenPaused = true;
}


void APlayerCharacterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PossessedPlayer = Cast<APlayerCharacter>(InPawn);
}


void APlayerCharacterController::OnUnPossess()
{
	Super::OnUnPossess();

	PossessedPlayer = nullptr;
}


void APlayerCharacterController::EnableHUD(bool IsEnabled)
{
	if (PlayerUIInstance)
	{
		if (IsEnabled)
		{
			PlayerUIInstance->SetVisibility(ESlateVisibility::Visible);
			UE_LOG(LogTemp, Warning, TEXT("Unhide Player UI"));
		}
		else
		{
			PlayerUIInstance->SetVisibility(ESlateVisibility::Hidden);
			UE_LOG(LogTemp, Warning, TEXT("Hide Player UI"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No PlayerUIInstance found"));
	}
}


void APlayerCharacterController::EnableControls(bool IsEnabled)
{
	if (IsEnabled)
	{
		EnableInput(this);
	}
	else
	{
		DisableInput(this);
	}
}


void APlayerCharacterController::MoveForward(float AxisValue)
{
	if (PossessedPlayer && AxisValue != 0)
		PossessedPlayer->AddMovementInput(PossessedPlayer->GetActorForwardVector(), AxisValue);
}


void APlayerCharacterController::MoveRight(float AxisValue)
{
	if (PossessedPlayer && AxisValue != 0)
		PossessedPlayer->AddMovementInput(PossessedPlayer->GetActorRightVector(), AxisValue);
}


void APlayerCharacterController::SprintStart()
{
	if (!PossessedPlayer)
		return;

	PossessedPlayer->bIsSprinting = true;
}


void APlayerCharacterController::SprintEnd()
{
	if (!PossessedPlayer)
		return;

	PossessedPlayer->bIsSprinting = false;
}


void APlayerCharacterController::JumpStart()
{
	if (!PossessedPlayer)
		return;

	PossessedPlayer->Jump();
}


void APlayerCharacterController::JumpEnd()
{
	if (!PossessedPlayer)
		return;

	PossessedPlayer->StopJumping();
}


void APlayerCharacterController::MouseX(float AxisValue)
{
	if (PossessedPlayer && AxisValue != 0)
		PossessedPlayer->AddControllerYawInput(AxisValue);
}


void APlayerCharacterController::MouseY(float AxisValue)
{
	if (PossessedPlayer && AxisValue != 0)
		PossessedPlayer->AddControllerPitchInput(AxisValue);
}


void APlayerCharacterController::ThrowCard()
{
	if (!PossessedPlayer)
		return;

	PossessedPlayer->ThrowCard();
}


void APlayerCharacterController::ActivateChainLightning()
{
	if (!PossessedPlayer)
		return;

	PossessedPlayer->ChainLightning();
}


void APlayerCharacterController::ChangeConstruct()
{
	if (!PossessedPlayer)
		return;

	PossessedPlayer->ChangeConstruct();
}


void APlayerCharacterController::ActivateLightningConstruct()
{
	if (!PossessedPlayer)
		return;

	PossessedPlayer->LightningConstruct();
}


void APlayerCharacterController::LightningConstructBreak()
{
	if (!PossessedPlayer)
		return;

	PossessedPlayer->DestroyConstructs();
}


void APlayerCharacterController::PauseGame()
{
	ALevelGameMode* LevelGameMode = Cast<ALevelGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (LevelGameMode)
	{
		if (LevelGameMode->IsPaused())
		{
			LevelGameMode->SetGamePause(false);
		}
		else
		{
			LevelGameMode->SetGamePause(true);
		}
	}
}
