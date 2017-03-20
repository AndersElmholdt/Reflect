// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LaserBouncer.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULaserBouncer : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class REFLECT_API ILaserBouncer
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	/**
	* Called when a Laser collides with this object.
	* @param Laser		The laser object that collided with this object.
	* @param HitNormal	The normal vector from the collision.
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void LaserHit(ALaserBase* Laser, FVector HitNormal);
};
