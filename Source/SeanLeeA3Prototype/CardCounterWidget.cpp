// Fill out your copyright notice in the Description page of Project Settings.


#include "CardCounterWidget.h"


void UCardCounterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	APlayerCharacter* PlayerRef = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	
	if (IsValid(PlayerRef))
	{
		PlayerRef->CardCountChangedDelegate.AddDynamic(this, &UCardCounterWidget::OnCardCounterChanged);	
	}
}


void UCardCounterWidget::OnCardCounterChanged(int CurrentCardCount, int MaxCardCount)
{
	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);

	CurrentCardCountLabel->SetText(FText::AsNumber(CurrentCardCount, &Opts));
	MaxCardCountLabel->SetText(FText::AsNumber(MaxCardCount, &Opts));
}
