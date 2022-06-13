// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NonAttachable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNonAttachable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SEANLEEA3PROTOTYPE_API INonAttachable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "NonAttachable")
	void HitNonAttachable(UStaticMeshComponent* CollidingObject);
};
