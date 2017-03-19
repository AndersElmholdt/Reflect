// Fill out your copyright notice in the Description page of Project Settings.

#include "Reflect.h"
#include "LaserBase.h"
#include "LaserBouncer.h"
#include "LaserAffector.h"


ALaserBase::ALaserBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	// Default values
	InitialSpeed = 600;
	MinSpeed = 300;
	MaxBounces = 5;

	InitialLifeSpan = 0;
	NumberOfBounces = 0;

	// Setup components
	CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Collision"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ALaserBase::OnBeginOverlap);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ALaserBase::OnEndOverlap);
	RootComponent = CollisionComp;

	TrailPCS = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("Trail"));
	TrailPCS->bAutoActivate = false;
	TrailPCS->bAutoDestroy = false;
	TrailPCS->SetupAttachment(RootComponent);

	ExplosionPCS = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("Explosion"));
	ExplosionPCS->bAutoActivate = false;
	ExplosionPCS->bAutoDestroy = false;
	ExplosionPCS->SetupAttachment(RootComponent);

	FirePCS = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("Fire"));
	FirePCS->bAutoActivate = false;
	FirePCS->bAutoDestroy = false;
	FirePCS->SetupAttachment(RootComponent);

	AudioComp = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("AudioComp"));
	AudioComp->bAutoActivate = false;
	AudioComp->bAutoDestroy = false;
	AudioComp->SetupAttachment(RootComponent);

	LightComp = ObjectInitializer.CreateDefaultSubobject<UPointLightComponent>(this, TEXT("LightComp"));
	LightComp->RelativeLocation = FVector(-20, 0, 0);
	LightComp->SetupAttachment(RootComponent);
}

void ALaserBase::BeginPlay()
{
	Super::BeginPlay();

	// Sets the start velocity and activates the trail particles
	Velocity = GetActorForwardVector() * InitialSpeed;
	TrailPCS->ActivateSystem();

	// TODO: Add Fire particles, and Audio
}

void ALaserBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Adjust the direction of the projectile
	CalculateNewDirection(DeltaSeconds);

	// Check if projectile should be killed
	if (Velocity.SizeSquared() < FMath::Square(MinSpeed))
	{
		Kill(false);
	}
	if (NumberOfBounces > MaxBounces)
	{
		Kill(true);
	}

	// Adjust the location according to the velocity
	SetActorLocation(GetActorLocation() + Velocity * DeltaSeconds, true);
}

// Force inline to optimize performance
FORCEINLINE
void ALaserBase::CalculateNewDirection(float DeltaSeconds)
{
	if (bIsChangingDirection)
	{
		const FVector NormalizedVelocity = Velocity.GetSafeNormal();
		const float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(NormalizedVelocity, TargetDir)));

		// Snap direction to target direction if the current angle is smaller than can be rotated in this tick
		if (Angle < DirectionChangeSpeed * DeltaSeconds)
		{
			Velocity = TargetDir * StartSpeed;
		}
		else
		{
			// Rotate the direction toward the target direction, aslong as the angle is not completely straight on
			if (!FMath::IsWithin(Angle, 179.0f, 181.0f) && !FMath::IsWithin(Angle, -1.0f, 1.0f))
			{
				FVector Orthogonal = FVector::CrossProduct(NormalizedVelocity, TargetDir);
				Velocity = Velocity.RotateAngleAxis(FMath::Sign(Angle) * DirectionChangeSpeed * DeltaSeconds, Orthogonal);
			}
			else
			{
				// Interpolate between the current velocity and the target direction
				Velocity = FMath::Lerp(Velocity, TargetDir * StartSpeed, (DirectionChangeSpeed / 180) * DeltaSeconds);
			}
		}
	}
}

int ALaserBase::GetNumberOfBounces() const
{
	return NumberOfBounces;
}

FVector ALaserBase::GetVelocity() const
{
	return Velocity;
}

void ALaserBase::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Check if the object it collided with implements the ILaserBouncer interface
	if (Other->GetClass()->ImplementsInterface(ULaserBouncer::StaticClass()))
	{
		// Call ILaserBouncer's OnHit function
		ILaserBouncer::Execute_OnHit(Other, this, HitNormal);
	}
	else
	{
		Kill(true);
	}
}

void ALaserBase::Bounce(FVector HitNormal, float BounceSpeed)
{
	FVector ReflectedVelocity = BounceSpeed * (-2 * FVector::DotProduct(Velocity, HitNormal) * HitNormal + Velocity);
	Velocity = ReflectedVelocity;
	ReflectedVelocity.Normalize();
	SetActorRotation(ReflectedVelocity.Rotation());
	NumberOfBounces++;
}

void ALaserBase::Kill(bool Explode)
{
	Destroy();
	// TODO: Add particle death if explode is true
}

void ALaserBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the object it overlaps with implements the ILaserAffector interface
	if (OtherActor->GetClass()->ImplementsInterface(ULaserAffector::StaticClass()))
	{
		// Call ILaserAffector's OnBeginOverlap function
		ILaserAffector::Execute_OnBeginOverlap(OtherActor, this);
	}
}

void ALaserBase::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check if the object it overlaps with implements the ILaserAffector interface
	if (OtherActor->GetClass()->ImplementsInterface(ULaserAffector::StaticClass()))
	{
		// Call ILaserAffector's OnEndOverlap function
		ILaserAffector::Execute_OnEndOverlap(OtherActor, this);
	}
}

void ALaserBase::ChangeDirection(FVector TargetDirection, float AngleSpeed)
{
	TargetDir = TargetDirection.GetSafeNormal();
	DirectionChangeSpeed = AngleSpeed;
	bIsChangingDirection = true;
	StartSpeed = Velocity.Size();
}

void ALaserBase::StopChangeDirection()
{
	bIsChangingDirection = false;
}
