// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFStateMachine.h"
#include "EngineUtils.h"

UCTFStateMachine::UCTFStateMachine()
{
}

UCTFStateMachine::~UCTFStateMachine()
{
	cleanup();
}

void UCTFStateMachine::HandleState(ACTF_AICharacter* seeker, float deltaTime)
{
	if (seeker->HasFlag())
	{

	}
	m_CurrentState->RunState(seeker, deltaTime);
}

void UCTFStateMachine::ChangeToState(UCTFState* state)
{
	if (m_CurrentState != state)
	{
		state->OnStateExit();
		m_CurrentState = state;
	}
}

void UCTFStateMachine::init(AActor* player, AActor* flag)
{
	if (!m_IsInit)
	{
		p_Player = player;
		p_Flag = flag;
		APathfindingManager* pathfinder = nullptr;
		for (TActorIterator<APathfindingManager> it(player->GetWorld()); it; ++it)
		{
			pathfinder = *it;
			break;
		}
		check(IsValid(pathfinder));
		UCTFPursuitFlagState* pursuitFlagState = NewObject<UCTFPursuitFlagState>();
		pursuitFlagState->SetPathfinder(pathfinder);
		pursuitFlagState->SetTarget(p_Flag);
		m_States.Add(pursuitFlagState);
		ChangeToState(pursuitFlagState);
	}
}

void UCTFStateMachine::cleanup()
{
}

void UCTFReturnAndProtectFlagState::RunState(ACTF_AICharacter* controlledActor, float deltaTime)
{

}
UCTFReturnAndProtectFlagState::UCTFReturnAndProtectFlagState()
{

}
UCTFReturnAndProtectFlagState::~UCTFReturnAndProtectFlagState()
{

}

UCTFState::UCTFState()
{

}
UCTFState::~UCTFState()
{
}


UPathfindingCTFState::~UPathfindingCTFState()
{
}

void UPathfindingCTFState::SetTarget(AActor* target)
{
	m_Target = target;
	m_TimeSincePathFound = MAX_FLT;
}

void UPathfindingCTFState::SetPathfinder(APathfindingManager* finder)
{
	pathfinder = finder;
	m_TimeSincePathFound = MAX_FLT;
}

void UPathfindingCTFState::SetTimeBetweenDjikstra(float time)
{
	m_DeltaTimePerDjikstra = time;
}

UPathfindingCTFState::UPathfindingCTFState()
{

}

UCTFPursuitFlagState::~UCTFPursuitFlagState()
{
}

void UCTFPursuitFlagState::OnStateExit()
{
}

UCTFPursuitFlagState::UCTFPursuitFlagState()
{
	
}

void UCTFPursuitFlagState::RunState(ACTF_AICharacter* controlledActor, float deltaTime)
{
	if (m_TimeSincePathFound > 1.0f && m_Target != nullptr && pathfinder != nullptr) //add check if player position and last path node distance greater than x
	{
		TArray<FVector> newPath = pathfinder->getPathFromToDjikstra(controlledActor->GetActorLocation(), m_Target->GetActorLocation());
		controlledActor->SetPathToFollow(newPath);
		m_TimeSincePathFound = 0.0f;
	}
	m_TimeSincePathFound += deltaTime;
}

void UCTFReturnAndProtectFlagState::OnStateExit()
{
}
