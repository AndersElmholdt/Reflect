// Fill out your copyright notice in the Description page of Project Settings.

#include "Reflect.h"
#include "SoundControlFunctions.h"

void USoundControlFunctions::SetSoundClassVolume(USoundClass* TargetSoundClass, float NewVolume)
{
	TargetSoundClass->Properties.Volume = NewVolume;
}

float USoundControlFunctions::GetSoundClassVolume(USoundClass* TargetSoundClass)
{
	if (TargetSoundClass)
	{
		TargetSoundClass->Properties.Volume;
	}
	return 0.0f;
}
