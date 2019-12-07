// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFindingActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "PathfindingManager.h"
#include "Engine/World.h"
#include "EngineUtils.h"


void APathFindingActor::SetPathToFollow(const TArray<FVector>& newPath)
{
	Path = newPath;
	NeedNewDestination = true;
}

void APathFindingActor::AddToPathFollow(const TArray<FVector>& newPath)
{
	for (const FVector& pos : newPath)
	{
		Path.Add(pos);
	}
}

void APathFindingActor::BeginPlay()
{
	Super::BeginPlay();
	for (TActorIterator<APathfindingManager> it(GetWorld()); it; ++it)
	{
		pathfinder = *it;
		break;
	}
}

void APathFindingActor::Tick(float DeltaTime)
{
	Seek();
	Avoid();
	Super::Tick(DeltaTime);
	if (NeedNewDestination)
	{
		if (Path.Num() > 0)
		{
			TargetPos.Emplace(Path[0]);
			Path.RemoveAt(0);
			NeedNewDestination = false;
		}
		else
		{
			TargetPos.Reset();
		}
	}
	else
	{
		if (TargetPos.IsSet())
		{
			float distanceToDestination = (Position - TargetPos.GetValue()).Size();
			if (distanceToDestination <= ReachRadius)
			{
				NeedNewDestination = true;
			}
		}
	}
}

void APathFindingActor::Avoid()
{
	if (pathfinder == nullptr)
	{
		return;
	}
	FVector result = FVector::ZeroVector;
	for (int i = 0; i < pathfinder->Walls.Num(); i++)
	{
		AActor* wall = pathfinder->Walls[i];
		if (FVector::DistSquared(wall->GetActorLocation(), GetActorLocation()) < AvoidDetectionRange * AvoidDetectionRange)
		{
			float dist = FVector::Distance(wall->GetActorLocation(), GetActorLocation());
			float dirMultiplier = 1.0f;
			if (FVector::DotProduct(FVector::CrossProduct(GetActorForwardVector(), FVector::UpVector), wall->GetActorLocation() - GetActorLocation()) > 0.0f)
			{
				dirMultiplier = -1.0f;
			}
			result -= FVector::CrossProduct(GetActorLocation() - wall->GetActorLocation(), FVector::UpVector) * (dist / AvoidDetectionRange) * dirMultiplier;
		}
	}
	targetDir += result.GetSafeNormal() * AvoidStrength;
}
