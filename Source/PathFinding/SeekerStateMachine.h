// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include <PathfindingManager.h>
#include "SeekerStateMachine.generated.h"

class ASeeker;

UCLASS()
class PATHFINDING_API UState : public UObject
{
	GENERATED_BODY()
public:

	enum class States
	{
		PATROL,
		PURSUIT
	};

	virtual void RunState(ASeeker* seeker, float deltaTime) {}

	UState();
	~UState();
};

UCLASS(BlueprintType)
class PATHFINDING_API UPatrolState : public UState
{
	GENERATED_BODY()
public:
	bool m_HasReturnedToLoop = false;
	float m_FieldOfViewAngle = 60.0f;  //half size
	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> m_Positions = TArray<FVector>();
	UPROPERTY(BlueprintReadWrite)
	float m_SeeingDistance = 400.0f;//distance to start pursuit

	void RunState(ASeeker* seeker, float deltaTime) override;

	void SetTarget(APawn* target);

	bool ShouldPursueTarget(ASeeker* seeker);
private:
	APawn* m_Target;
};

UCLASS(BlueprintType)
class PATHFINDING_API UPursuitState : public UState
{
	GENERATED_BODY()
public:
	//APathfindingManager* pathfinder;
	UPursuitState();
	UPROPERTY(BlueprintReadWrite)
	FVector m_LastTargetPos;
	UPROPERTY(BlueprintReadWrite)
	float m_TimeSinceSeenPlayer = 0.0f;
	UPROPERTY(BlueprintReadWrite)
	float m_MaxTimeBeforeQuit = 4.0f;

	float m_FleeingDistance = 800.0f;//distance to losing sight

	void RunState(ASeeker* seeker, float deltaTime) override;

	bool ShouldExitPursuit();

	void SetTarget(APawn* target);

	void SetPathfinder(APathfindingManager* pathfinder);
private:
	APawn* m_Target = nullptr;
	APathfindingManager* pathfinder = nullptr;

	float timeSincePathFound = MAX_FLT;
};


UCLASS()
class PATHFINDING_API USeekerStateMachine : public UObject
{
	GENERATED_BODY()
public:
	USeekerStateMachine();

	UFUNCTION(BlueprintCallable)
	void init(APawn* player);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	APawn* m_Target;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ASeeker* m_Seeker;

	UPROPERTY(BlueprintReadWrite)
	TArray<UState*> m_States;
	UPROPERTY(BlueprintReadWrite)
	UState* m_CurrentState = nullptr;

	UFUNCTION(BlueprintCallable)
	void HandleState(ASeeker* seeker, float deltaTime);
	
	UFUNCTION(BlueprintCallable)
	void ChangeToState(UState* state);
private:
	bool m_IsInit = false;
};