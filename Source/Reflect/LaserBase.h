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

	// The initial speed of the bullet.
	UPROPERTY(EditDefaultsOnly, Category = "Laser")
	float InitialSpeed;

	// The minimum speed of the bullet before it gets killed.
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

	// Gets the amount of times the projectile has bounced
	UFUNCTION(BlueprintCallable, Category = "Laser")
	int GetNumberOfBounces() const;

	// Gets the current velocity of the projectile
	UFUNCTION(BlueprintCallable, Category = "Laser")
	FVector GetVelocity() const;

	// Bounces the projectile
	UFUNCTION(BlueprintCallable, Category = "Laser")
	void Bounce(FVector HitNormal, float BounceSpeed = 1.0f);

	/**
	* Kills the projectile.
	* @param Explode			Determines whether the kill should create exploding particles
	*/
	UFUNCTION(BlueprintCallable, Category = "Laser")
	void Kill(bool Explode);

	/**
	* Changes the direction of the projectile.
	* @param TargetDirection	The direction that the projectile moves towards.
	* @param AngleSpeed		The speed at which the projectile changes direction.
	*/
	UFUNCTION(BlueprintCallable, Category = "Laser")
	void ChangeDirection(FVector TargetDirection, float AngleSpeed);

	/**
	* Stops the current change of direction.
	*/
	UFUNCTION(BlueprintCallable, Category = "Laser")
	void StopChangeDirection();

	/***************************************/
	/* Inaccessible members                */
	/***************************************/

private:

	// Amount of times the projectile has bounced.
	int NumberOfBounces;

	// The current velocity of the projectile.
	FVector Velocity;

	// The direction that the projectile is moving towards.
	FVector TargetDir;

	// The speed at which the projectile changes direction.
	float DirectionChangeSpeed;

	// Is the projectile currently changing direction.
	bool bIsChangingDirection;

	// The speed of the projectile before the ChangeDirection function is called.
	float StartSpeed;

	/**
	* Calculates a new direction, determines by TargetDir and DirectionChangeSpeed.
	* @param DeltaSeconds		Amount of seconds passed since last tick was called.
	*/
	void CalculateNewDirection(float DeltaSeconds);
};
