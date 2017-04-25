// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "FMODEvent.h"
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

	// Angle to clamp to when the laser bounces.
	UPROPERTY(EditDefaultsOnly, Category = "Laser")
	int BounceClampAngle;

	UPROPERTY(EditDefaultsOnly, Category = "Laser")
	UFMODEvent* LaserExplosionEvent;

	// Called when the projectile starts overlapping another object.
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called when the projectile ends overlapping another object.
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/**
	 * Clamps an incoming vector to a specific angle.
	 * @param InVector			The incoming vector.
	 * @param ForwardVector		Vector pointing forward.
	 * @param ClampAngle		The angle to clamp to.
	 */
	UFUNCTION()
	FVector ClampVectorAngle(FVector InVector, FVector ForwardVector, int ClampAngle);

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
	 * @param ClampAngle		Clamps the reflected angle in multiples of this.
	 */
	UFUNCTION(BlueprintCallable, Category = "Laser")
	void Bounce(FVector HitNormal, float BounceSpeed = 1.0f, float ClampAngle = 0.0f);

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

	/**
	 * Sets a goal direction of the laser.
	 * @param NewGoalDirection	The direction the laser should move towards.
	 * @param TransitionTime	How fast it should transition into the new direction.
	 */
	UFUNCTION(BlueprintCallable, Category = "Laser")
	void SetGoalDirection(FVector NewGoalDirection, float TransitionTime);

	UFUNCTION(BlueprintImplementableEvent, Category = "Laser")
	void OnExplode();

	/***************************************/
	/* Inaccessible members                */
	/***************************************/

private:

	// Updates the velocity of the laser, to move towards the goal
	void MoveTowardsGoal();

	// Array of affectors that the laser is currently overlapping.
	TArray<AActor*> LaserAffectors;

	// Amount of times the projectile has bounced.
	int NumberOfBounces;

	// The current velocity of the projectile.
	FVector Velocity;

	// The goal's normalized direction vector
	FVector GoalDirection;

	// The time it takes for the laser to redirect towards the goal
	float GoalTransitionTime;

	// The total amount of angle between the starting direction, and the goal direction
	float TotalGoalAngle;

	// How often the velocity is updated in MoveTowardsGoal()
	const float GoalMovementTick = 1.0f / 144.0f;

	// Handles the goal updates
	FTimerHandle GoalTimer;

	bool bIsAlive = true;

	void DestroyLaser();

};
