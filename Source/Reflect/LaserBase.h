// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LaserBase.generated.h"

UCLASS()
class REFLECT_API ALaserBase : public AActor
{
	GENERATED_UCLASS_BODY()

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	// Called when the object is spawned.
	virtual void BeginPlay() override;

	// Called when a blocking hit is detected.
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	// The initial speed of the projectile.
	UPROPERTY(EditDefaultsOnly, Category = "Laser")
	float InitialSpeed;

	// The maximum speed of the projectile.
	UPROPERTY(EditDefaultsOnly, Category = "Laser")
	float MaxSpeed;

	// The minimum speed of the projectile before it gets killed.
	UPROPERTY(EditDefaultsOnly, Category = "Laser")
	float MinSpeed;

	// The maximum amount of bounces before the projectile is killed.
	UPROPERTY(EditDefaultsOnly, Category = "Laser")
	int MaxBounces;

	// Called when the projectile starts overlapping another object.
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called when the projectile ends overlapping another object.
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/***************************************/
	/* Components                          */
	/***************************************/

	// Trail particle system component
	UPROPERTY(VisibleDefaultsOnly)
	UParticleSystemComponent* TrailPCS;

	// Explosion particle system component
	UPROPERTY(VisibleDefaultsOnly)
	UParticleSystemComponent* ExplosionPCS;

	// Fire particle system component
	UPROPERTY(VisibleDefaultsOnly)
	UParticleSystemComponent* FirePCS;

	// Audio component
	UPROPERTY(VisibleDefaultsOnly)
	UAudioComponent* AudioComp;

	// Collision component
	UPROPERTY(EditDefaultsOnly, Category = "Laser|Collision")
	USphereComponent* CollisionComp;

	// Light component
	UPROPERTY(EditDefaultsOnly, Category = "Laser|Light")
	UPointLightComponent* LightComp;

	// Trail particle system
	UPROPERTY(EditDefaultsOnly, Category = "Laser|Effects")
	UParticleSystem* TrailFX;

	// Explosion particle system
	UPROPERTY(EditDefaultsOnly, Category = "Laser|Effects")
	UParticleSystem* ExplosionFX;

	// Fire particle system
	UPROPERTY(EditDefaultsOnly, Category = "Laser|Effects")
	UParticleSystem* FireFX;

	// Trail sound cue
	UPROPERTY(EditDefaultsOnly, Category = "Laser|Effects")
	USoundCue* TrailSound;

	// Explosion sound cue
	UPROPERTY(EditDefaultsOnly, Category = "Laser|Effects")
	USoundCue* ExplosionSound;

	// Fire sound cue
	UPROPERTY(EditDefaultsOnly, Category = "Laser|Effects")
	USoundCue* FireSound;

	/***************************************/
	/* Blueprint Functions                 */
	/***************************************/

	//

	// Gets the amount of times the projectile has bounced
	UFUNCTION(BlueprintCallable, Category = "Laser")
	int GetNumberOfBounces() const;

	// Gets the current velocity of the projectile
	UFUNCTION(BlueprintCallable, Category = "Laser")
	FVector GetVelocity() const;

	// Gets the current speed of the projectile
	UFUNCTION(BlueprintCallable, Category = "Laser")
	float GetSpeed() const;

	// Gets the current squared speed of the projectile
	UFUNCTION(BlueprintCallable, Category = "Laser")
	float GetSquaredSpeed() const;

	/**
	 * Sets the velocity of the projectile.
	 * @param NewVelocity		The new velocity that the projectile should have.
	 */
	UFUNCTION(BlueprintCallable, Category = "Laser")
	void SetVelocity(FVector NewVelocity);

	/**
	 * Sets the speed of the projectile.
	 * @param NewSpeed			The new speed of the projectile.
	 */
	UFUNCTION(BlueprintCallable, Category = "Laser")
	void SetSpeed(float NewSpeed);

	/**
	 * Sets the maximum speed that the projectile can have.
	 * @param NewMaxSpeed		The new maximum speed that the projectile can have.
	 */
	UFUNCTION(BlueprintCallable, Category = "Laser")
	void SetMaxSpeed(float NewMaxSpeed);

	/**
	 * Sets the minimum speed that the projectile can have before getting killed.
	 * @param NewMinSpeed		The new minimum speed that the projectile can have.
	 */
	UFUNCTION(BlueprintCallable, Category = "Laser")
	void SetMinSpeed(float NewMinSpeed);

	/**
	 * Bounces the projectile.
	 * @param HitNormal			The normal vector of the impact.
	 * @param BounceSpeed		The relative speed of the projectile after the bounce.
	 */
	UFUNCTION(BlueprintCallable, Category = "Laser")
	void Bounce(FVector HitNormal, float BounceSpeed = 1.0f);

	/**
	* Kills the projectile.
	* @param Explode			Determines whether the kill should create exploding particles
	*/
	UFUNCTION(BlueprintCallable, Category = "Laser")
	void Kill(bool Explode);

	/**
	* Rotates the direction of the projectile towards a new direction.
	* @param NewDirection		The direction that the projectile moves towards.
	* @param MaxAngle			The maximum allowed rotation angle.
	*/
	UFUNCTION(BlueprintCallable, Category = "Laser")
	void RotateVelocity(FVector NewDirection, float MaxAngle);

	/**
	 * Adds a force to the projectile.
	 * @param ForceDirection	The direction of the force.
	 * @param Intensity			The intensity of the force.
	 */
	UFUNCTION(BlueprintCallable, Category = "Laser")
	void AddForce(FVector ForceDirection, float Intensity);

	/***************************************/
	/* Inaccessible members                */
	/***************************************/

private:

	// Array of affectors that the laser is currently overlapping.
	TArray<AActor*> LaserAffectors;

	// Amount of times the projectile has bounced.
	int NumberOfBounces;

	// The current velocity of the projectile.
	FVector Velocity;
};
