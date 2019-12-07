// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SteeringActor.h"
#include "PathFindingActor.generated.h"

class APathfindingManager;

UCLASS()
class PATHFINDING_API  APathFindingActor : public ASteeringActor
{
	GENERATED_BODY()
public:	

	TArray<FVector> GetPath() { return Path; }

	UFUNCTION(BlueprintCallable)
	void SetPathToFollow(const TArray<FVector>& newPath);

	UFUNCTION(BlueprintCallable)
	void AddToPathFollow(const TArray<FVector>& newPath);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool NeedNewDestination = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APathfindingManager* pathfinder = nullptr;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	void Avoid();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReachRadius = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AvoidDetectionRange = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> Path;
};
