// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PathFindingActor.h"
#include "SeekerStateMachine.h"
#include "Seeker.generated.h"

UCLASS()
class PATHFINDING_API ASeeker : public APathFindingActor
{
	GENERATED_BODY()
	
public:
	ASeeker();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USeekerStateMachine* m_StateMachine;

	APathfindingManager* pathfinder;

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

};

