// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCharacter.h"
#include "Components/Textblock.h"
#include "Components/Image.h"
#include "LCSelectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API ULCSelectionWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, Category = "LCSelectionWidget")
	UTexture2D* WallImageRef;
	UPROPERTY(EditAnywhere, Category = "LCSelectionWidget")
	UTexture2D* BridgeImageRef;


protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* ConstructImage;


protected:
	void NativeConstruct() override;

	UFUNCTION()
	void OnConstructChanged(bool IsWall);
};
