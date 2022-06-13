// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "EnemyHealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class SEANLEEA3PROTOTYPE_API UEnemyHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	
	AEnemyBase* EnemyRef = nullptr;

	
public:
	UFUNCTION()
	void OnHealthChanged(float CurrentHealth, float MaxHealth);

	
protected:
	void NativeConstruct() override;
};
