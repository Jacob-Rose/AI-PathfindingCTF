// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "CTF_AICharacter.h"
#include "PathfindingManager.h"
#include "CTFStateMachine.generated.h"

UCLASS()
class PATHFINDING_API UCTFState : public UObject
{
	GENERATED_BODY()
public:

	virtual void RunState(ACTF_AICharacter* controlledActor, float deltaTime) {}

	virtual void OnStateExit() {}

	UCTFState();
	~UCTFState();
};

UCLASS()
class PATHFINDING_API UPathfindingCTFState : public UCTFState
{
	GENERATED_BODY()
public:

	UPathfindingCTFState();
	~UPathfindingCTFState();

	void SetTarget(AActor* target);
	void SetPathfinder(APathfindingManager* pathfinder);
	void SetTimeBetweenDjikstra(float time);
protected:
	AActor* m_Target;
	APathfindingManager* pathfinder = nullptr;
	float m_DeltaTimePerDjikstra = 0.5f;
	float m_TimeSincePathFound = MAX_FLT;
};

UCLASS()
class PATHFINDING_API UCTFPursuitFlagState : public UPathfindingCTFState
{
	GENERATED_BODY()
public:

	virtual void RunState(ACTF_AICharacter* controlledActor, float deltaTime);

	virtual void OnStateExit() override;

	UCTFPursuitFlagState();
	~UCTFPursuitFlagState();

	void SetPlayer(ACTF_Character* player);
protected:
	ACTF_Character* p_Player;
};

UCLASS()
class PATHFINDING_API UCTFReturnAndProtectFlagState : public UPathfindingCTFState
{
	GENERATED_BODY()
public:

	virtual void RunState(ACTF_AICharacter* controlledActor, float deltaTime);

	virtual void OnStateExit() override;

	UCTFReturnAndProtectFlagState();
	~UCTFReturnAndProtectFlagState();
private:

};


UCLASS()
class PATHFINDING_API UCTFStateMachine : public UObject
{
	GENERATED_BODY()
public:
	UCTFStateMachine();
	~UCTFStateMachine();

	UFUNCTION(BlueprintCallable)
	void HandleState(ACTF_AICharacter* seeker, float deltaTime);

	UFUNCTION(BlueprintCallable)
	void ChangeToState(UCTFState* state);

	UFUNCTION(BlueprintCallable)
	void init(ACTF_Character* player, AActor* flag);
	void cleanup();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ACTF_Character* p_Player = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		AActor* p_PlayerBase = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		AActor* p_MyBase = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		AActor* p_Flag = nullptr;
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCTFPursuitFlagState* pursuitState;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCTFReturnAndProtectFlagState* returnAndProtectState;
	UCTFState* m_CurrentState = nullptr;

	bool m_IsInit = false;
};
