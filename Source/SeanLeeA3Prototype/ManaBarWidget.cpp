// Fill out your copyright notice in the Description page of Project Settings.


#include "ManaBarWidget.h"


void UManaBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerCharacter* PlayerRef = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	
	if (IsValid(PlayerRef))
	{
		PlayerRef->ManaChangedDelegate.AddDynamic(this, &UManaBarWidget::OnManaChanged);	
	}
}


void UManaBarWidget::OnManaChanged(float CurrentMana, float MaxMana)
{
	float Percentage = CurrentMana / MaxMana;
	
	ManaBar->SetPercent(Percentage);
	
	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);

	CurrentManaLabel->SetText(FText::AsNumber(CurrentMana, &Opts));
	MaxManaLabel->SetText(FText::AsNumber(MaxMana, &Opts));
}

