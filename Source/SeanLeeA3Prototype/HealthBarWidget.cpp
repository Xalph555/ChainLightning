// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"


void UHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerCharacter* PlayerRef = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	
	if (IsValid(PlayerRef))
	{
		PlayerRef->HealthChangedDelegate.AddDynamic(this, &UHealthBarWidget::OnHealthChanged);	
	}
}


void UHealthBarWidget::OnHealthChanged(float CurrentHealth, float MaxHealth)
{
	float Percentage = CurrentHealth / MaxHealth;
	
	HealthBar->SetPercent(Percentage);
	
	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);

	CurrentHealthLabel->SetText(FText::AsNumber(CurrentHealth, &Opts));
	MaxHealthLabel->SetText(FText::AsNumber(MaxHealth, &Opts));
}
