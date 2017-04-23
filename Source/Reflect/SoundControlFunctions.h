// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SoundControlFunctions.generated.h"

/**
 * 
 */
UCLASS()
class REFLECT_API USoundControlFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Sound")
		static void SetSoundClassVolume(USoundClass* TargetSoundClass, float NewVolume);

	UFUNCTION(BlueprintCallable, Category = "Sound")
		static float GetSoundClassVolume(USoundClass* TargetSoundClass);
	
	
};
