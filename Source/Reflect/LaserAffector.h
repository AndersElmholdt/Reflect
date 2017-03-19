// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LaserAffector.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULaserAffector : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class REFLECT_API ILaserAffector
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	/**
	 * Called when a Laser starts overlapping with this object.
	 * @param Laser		The laser object that started overlapping this object.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginOverlap(ALaserBase* Laser);

	/**
	* Called when a Laser ends overlapping with this object.
	* @param Laser		The laser object that ended overlapping this object.
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnEndOverlap(ALaserBase* Laser);
};
