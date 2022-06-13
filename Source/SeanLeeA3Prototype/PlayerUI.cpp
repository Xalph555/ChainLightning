// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"
#include "LevelGameMode.h"
#include "PlayerCharacterController.h"


UPlayerUI::UPlayerUI(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	PauseUIInstance = nullptr;

	// get reference to pause menu
	static ConstructorHelpers::FClassFinder<UUserWidget> PauseBPClass(TEXT("/Game/Blueprints/UI/PlayerUI/BP_PauseUI"));
	
	if (PauseBPClass.Class)
	{
		PauseUIRef = PauseBPClass.Class;
	}

	GameOverUIInstance = nullptr;
	// get reference to gameover menu
	static ConstructorHelpers::FClassFinder<UUserWidget> GameOverBPClass(TEXT("/Game/Blueprints/UI/PlayerUI/BP_GameOverUI"));
	
	if (GameOverBPClass.Class)
	{
		GameOverUIRef = GameOverBPClass.Class;
	}
}


void UPlayerUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	ALevelGameMode* LevelGameMode = Cast<ALevelGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (LevelGameMode)
	{
		LevelGameMode->GamePausedDelegate.AddDynamic(this, &UPlayerUI::OnGamePaused);
		LevelGameMode->GameOverDelegate.AddDynamic(this, &UPlayerUI::OnGameOver);
	}
}


void UPlayerUI::OnGamePaused(bool IsPaused)
{
	// pause game
	if (IsPaused)
	{
		if (!IsValid(PauseUIInstance))
		{
			PauseUIInstance = CreateWidget<UPauseUI>(this, PauseUIRef);
		}

		PauseUIInstance->AddToViewport(0);
		
		FInputModeGameAndUI InputMode;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
	}
	// unpause game
	else
	{
		PauseUIInstance->RemoveFromParent();
		
		FInputModeGameOnly InputMode;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
	}
}


void UPlayerUI::OnGameOver()
{
	if (!IsValid(GameOverUIInstance))
	{
		GameOverUIInstance = CreateWidget<UGameOverUI>(this, GameOverUIRef);
	}

	// don't add game over UI more than once to viewport 
	if (GameOverUIInstance->IsInViewport())
		return;
	
	GameOverUIInstance->AddToViewport(0);

	FInputModeUIOnly InputMode;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
}