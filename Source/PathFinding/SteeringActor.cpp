// Fill out your copyright notice in the Description page of Project Settings.


#include "SteeringActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASteeringActor::ASteeringActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASteeringActor::BeginPlay()
{
	Super::BeginPlay();

	Position = GetActorLocation();
}

// Called every frame
void ASteeringActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// steering
	SteeringVelocity += (SteeringVelocity * DragForce * DeltaTime);
	SteeringVelocity += (Seek() * SeekStrength * DeltaTime);
	SteeringVelocity += (Avoid() * DeltaTime * AvoidStrength);

	// limit Speed
	if (SteeringVelocity.Size() > m_MaxSpeed)
	{
		SteeringVelocity = SteeringVelocity.GetSafeNormal() * m_MaxSpeed;
	}
}

FVector ASteeringActor::Seek()
{
	FVector dir = TargetPos - Position;
	float distance = dir.Size();
	dir.Z = 0; // consider only 2d plane
	dir = dir / dir.Size();

	const float maxSpeedDistance = 500.0f;
	float speedRatio = FMath::Clamp(distance / SeekDecelerationDistance, 0.0f, 1.0f);

	return dir * speedRatio;
}

FVector ASteeringActor::Avoid()
{
	FHitResult lftHit;
	bool lftHitBool;
	FHitResult rgtHit;
	bool rgtHitBool;
	FVector pos = GetActorLocation();
	FCollisionQueryParams cqp;
	cqp.AddIgnoredActor(this);
	DrawDebugSphere(GetWorld(), pos, 5.0f, 8, FColor::Blue, false, 1.0f);
	DrawDebugSphere(GetWorld(), pos + (GetActorForwardVector() * (Radius + 100.0f)), 5.0f, 8, FColor::Blue, false, 1.0f);
	FVector lftOffset = FVector::CrossProduct(GetActorForwardVector(), FVector::UpVector) * Radius;
	lftHitBool = GetWorld()->LineTraceSingleByObjectType(lftHit, pos + lftOffset,
		pos + (GetActorForwardVector() * (Radius + 100.0f)) + lftOffset, FCollisionObjectQueryParams::AllObjects, cqp);
	rgtHitBool = GetWorld()->LineTraceSingleByObjectType(rgtHit, pos - lftOffset,
		pos + (GetActorForwardVector() * (Radius + 100.0f)) - lftOffset, FCollisionObjectQueryParams::AllObjects, cqp);
	if (lftHitBool && rgtHitBool)
	{
		if (lftHit.Distance > rgtHit.Distance)
		{
			//turnleft
			return FVector::CrossProduct(GetActorForwardVector(), -FVector::UpVector);
		}
		else
		{
			//turnright
			return FVector::CrossProduct(GetActorForwardVector(), FVector::UpVector);
		}
	}
	else if (lftHitBool)
	{
		return FVector::CrossProduct(GetActorForwardVector(), -FVector::UpVector);
	}
	else if (rgtHitBool)
	{
		
		return FVector::CrossProduct(GetActorForwardVector(), FVector::UpVector);
	}
	return FVector::ZeroVector;
}
