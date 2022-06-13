// Fill out your copyright notice in the Description page of Project Settings.


#include "LCSelectionWidget.h"


void ULCSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerCharacter* PlayerRef = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	
	if (IsValid(PlayerRef))
	{
		PlayerRef->ConstructChangedDelegate.AddDynamic(this, &ULCSelectionWidget::OnConstructChanged);	
	}
}


void ULCSelectionWidget::OnConstructChanged(bool IsWall)
{
	if (!WallImageRef || !BridgeImageRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reference images for constructs not set"));
		return;
	}
	
	if (IsWall)
	{
		ConstructImage->SetBrushFromTexture(WallImageRef);
	}
	else
	{
		ConstructImage->SetBrushFromTexture(BridgeImageRef);
	}
}
