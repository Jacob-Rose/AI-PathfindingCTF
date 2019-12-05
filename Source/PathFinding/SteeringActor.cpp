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
	SteeringVelocity += (targetDir * SeekStrength * DeltaTime);
	//TODO make avoid work on targetdir
	SteeringVelocity += (Avoid() * DeltaTime * AvoidStrength);

	// limit Speed
	if (SteeringVelocity.Size() > m_MaxSpeed)
	{
		SteeringVelocity = SteeringVelocity.GetSafeNormal() * m_MaxSpeed;
	}
}

void ASteeringActor::Seek()
{
	FVector dir = TargetPos - Position;
	float distance = dir.Size();
	dir.Z = 0; // consider only 2d plane
	dir = dir / dir.Size();

	const float maxSpeedDistance = 500.0f;
	float speedRatio = FMath::Clamp(distance / SeekDecelerationDistance, 0.0f, 1.0f);

	targetDir = dir * speedRatio;
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
	//DrawDebugSphere(GetWorld(), pos, 5.0f, 8, FColor::Blue, false, 1.0f);
	//DrawDebugSphere(GetWorld(), pos + (GetActorForwardVector() * (Radius + 100.0f)), 5.0f, 8, FColor::Blue, false, 1.0f);
	FVector lftOffset = FVector::CrossProduct(GetActorForwardVector(), FVector::UpVector) * Radius;
	lftHitBool = GetWorld()->LineTraceSingleByChannel(lftHit, pos + lftOffset,
		pos + (GetActorForwardVector() * (Radius + 100.0f)) + lftOffset, ECollisionChannel::ECC_WorldDynamic, cqp);
	rgtHitBool = GetWorld()->LineTraceSingleByChannel(rgtHit, pos - lftOffset,
		pos + (GetActorForwardVector() * (Radius + 100.0f)) - lftOffset, ECollisionChannel::ECC_WorldDynamic, cqp);
	/*rgtHitBool = GetWorld()->LineTraceSingleByObjectType(rgtHit, pos - lftOffset,
		pos + (GetActorForwardVector() * (Radius + 100.0f)) - lftOffset, FCollisionObjectQueryParams::AllObjects, cqp);*/
	FVector result = FVector::ZeroVector;
	if (lftHitBool && rgtHitBool)
	{
		if (lftHit.Distance > rgtHit.Distance)
		{
			//turnleft
			result = FVector::CrossProduct(GetActorForwardVector(), -FVector::UpVector);
		}
		else
		{
			//turnright
			result = FVector::CrossProduct(GetActorForwardVector(), FVector::UpVector);
		}
	}
	else if (lftHitBool)
	{
		result = FVector::CrossProduct(GetActorForwardVector(), -FVector::UpVector);
	}
	else if (rgtHitBool)
	{
		
		result= FVector::CrossProduct(GetActorForwardVector(), FVector::UpVector);
	}
	return result;
}

FVector ASteeringActor::AvoidBullet()
{
	return FVector();
}
