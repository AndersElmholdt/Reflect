// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "EaseFunctions.generated.h"

/**
 * 
 */
UCLASS()
class REFLECT_API UEaseFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


	UFUNCTION(BlueprintCallable)
	static void EaseFloatTo(float Input, float Target, float TransitionTime, float TickTime);

	static void PerformEase(float Input, float Target, float TransitionTime, float TickTime);
	
};
