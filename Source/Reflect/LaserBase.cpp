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
	MinSpeed = -1;
	MaxSpeed = 800;
	MaxBounces = 5;
	BounceClampAngle = 5;

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

	for (AActor* Affector : LaserAffectors)
	{
		ILaserAffector::Execute_LaserTick(Affector, this, DeltaSeconds);
	}

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
		ILaserBouncer::Execute_LaserHit(Other, this, HitNormal);
	}
	else
	{
		Kill(true);
	}
}

void ALaserBase::Bounce(FVector HitNormal, float BounceSpeed, float ClampAngle)
{
	FVector ReflectedVelocity = BounceSpeed * (-2 * FVector::DotProduct(Velocity, HitNormal) * HitNormal + Velocity);

	// Clamp angle to improve user's shots
	if (ClampAngle != 0.0f)
	{
		ReflectedVelocity = ClampVectorAngle(ReflectedVelocity, HitNormal, ClampAngle);
	}

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
		ILaserAffector::Execute_LaserBeginOverlap(OtherActor, this);

		LaserAffectors.Add(OtherActor);
	}
}

void ALaserBase::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check if the object it overlaps with implements the ILaserAffector interface
	if (OtherActor->GetClass()->ImplementsInterface(ULaserAffector::StaticClass()))
	{
		// Call ILaserAffector's OnEndOverlap function
		ILaserAffector::Execute_LaserEndOverlap(OtherActor, this);

		LaserAffectors.Remove(OtherActor);
	}
}

void ALaserBase::RotateVelocity(FVector NewDirection, float MaxAngle)
{
	MaxAngle = FMath::Clamp(MaxAngle, 0.0f, 180.0f);
	
	const FVector NormalizedVelocity = Velocity.GetSafeNormal();
	const FVector NormalizedNewDirection = NewDirection.GetSafeNormal();
	const float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(NormalizedVelocity, NormalizedNewDirection)));

	// Snap direction to target direction if the current angle is smaller than max angle
	if (Angle <= MaxAngle)
	{
		Velocity = NormalizedNewDirection * Velocity.Size();
	}
	else
	{
		FVector Orthogonal = FVector::CrossProduct(NormalizedVelocity, NormalizedNewDirection).GetSafeNormal();
		if (Orthogonal.Equals(FVector::ZeroVector))
		{
			Orthogonal = FVector::UpVector;
		}
		
		Velocity = Velocity.RotateAngleAxis(FMath::Sign(Angle) * MaxAngle, Orthogonal);
	}
	SetActorRotation(Velocity.Rotation());
}

void ALaserBase::AddForce(FVector ForceDirection, float Intensity)
{
	Velocity = Velocity + ForceDirection.GetSafeNormal() * Intensity;

	if (Velocity.SizeSquared() > FMath::Square(MaxSpeed))
	{
		Velocity = Velocity.GetSafeNormal() * MaxSpeed;
	}
	SetActorRotation(Velocity.Rotation());
}

float ALaserBase::GetSpeed() const
{
	return Velocity.Size();
}

float ALaserBase::GetSquaredSpeed() const
{
	return Velocity.SizeSquared();
}

void ALaserBase::SetVelocity(FVector NewVelocity)
{
	if (NewVelocity.SizeSquared() < FMath::Square(MaxSpeed))
	{
		Velocity = NewVelocity;
	}
	else
	{
		Velocity = NewVelocity.GetSafeNormal() * MaxSpeed;
	}
	SetActorRotation(Velocity.Rotation());
}

void ALaserBase::SetSpeed(float NewSpeed)
{
	if (NewSpeed < MaxSpeed)
	{
		Velocity = Velocity.GetSafeNormal() * NewSpeed;
	}
	else
	{
		Velocity = Velocity.GetSafeNormal() * MaxSpeed;
	}
	SetActorRotation(Velocity.Rotation());
}

void ALaserBase::SetMaxSpeed(float NewMaxSpeed)
{
	MaxSpeed = NewMaxSpeed;
}

void ALaserBase::SetMinSpeed(float NewMinSpeed)
{
	MinSpeed = NewMinSpeed;
}

//FORCEINLINE
FVector ALaserBase::ClampVectorAngle(FVector InVector, FVector ForwardVector, int ClampAngle)
{
	if (ClampAngle > 0)
	{
		const FVector NormalizedInVector = InVector.GetSafeNormal();
		const FVector NormalizedForwardVector = ForwardVector.GetSafeNormal();
		const FVector UpVector = FVector::CrossProduct(NormalizedInVector, NormalizedForwardVector).GetSafeNormal();

		const float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(NormalizedInVector, NormalizedForwardVector)));
		const float SnappedAngle = FMath::GridSnap(Angle, ClampAngle);
		const float DiffAngle = Angle - SnappedAngle;

		return InVector.RotateAngleAxis(DiffAngle, UpVector);
	}
	return InVector;
}
