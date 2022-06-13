// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealthBarWidget.h"


void UEnemyHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}


void UEnemyHealthBarWidget::OnHealthChanged(float CurrentHealth, float MaxHealth)
{
	float Percentage = CurrentHealth / MaxHealth;
	
	HealthBar->SetPercent(Percentage);
}
