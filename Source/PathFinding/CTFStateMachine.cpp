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
	if (seeker->HasFlag)
	{
		ChangeToState(returnAndProtectState); //change to state checks if already the state
	}
	else
	{
		ChangeToState(pursuitState);
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

void UCTFStateMachine::init(ACTF_Character* player, AActor* flag)
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
		pursuitState = NewObject<UCTFPursuitFlagState>();
		returnAndProtectState = NewObject<UCTFReturnAndProtectFlagState>();
		returnAndProtectState->SetTarget(p_MyBase);
		returnAndProtectState->SetPathfinder(pathfinder);
		pursuitState->SetPathfinder(pathfinder);
		pursuitState->SetTarget(p_Flag);
		pursuitState->SetPlayer(p_Player);
		ChangeToState(pursuitState);
	}
}

void UCTFStateMachine::cleanup()
{
}

void UCTFReturnAndProtectFlagState::RunState(ACTF_AICharacter* controlledActor, float deltaTime)
{
	if (m_TimeSincePathFound > 1.0f && m_Target != nullptr && pathfinder != nullptr) //add check if player position and last path node distance greater than x
	{
		TArray<FVector> newPath = pathfinder->getPathFromToDjikstra(controlledActor->GetActorLocation(), m_Target->GetActorLocation());
		controlledActor->SetPathToFollow(newPath);
		m_TimeSincePathFound = 0.0f;
	}
	m_TimeSincePathFound += deltaTime;
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

void UCTFPursuitFlagState::SetPlayer(ACTF_Character* player)
{
	p_Player = player;
}

void UCTFPursuitFlagState::OnStateExit()
{
}

UCTFPursuitFlagState::UCTFPursuitFlagState()
{
	
}

void UCTFPursuitFlagState::RunState(ACTF_AICharacter* controlledActor, float deltaTime)
{
	if (p_Player->HasFlag)
	{
		if (FVector::Dist(controlledActor->GetActorLocation(), p_Player ->GetActorLocation()) < controlledActor->m_MaxShootDist)
		{
			FCollisionQueryParams cqp;
			cqp.AddIgnoredActor(p_Player);
			cqp.AddIgnoredActor(controlledActor);
			FHitResult hit;
			if (!GetWorld()->LineTraceSingleByChannel(hit, p_Player->GetActorLocation(), controlledActor->GetActorLocation(), ECollisionChannel::ECC_WorldDynamic, cqp))
			{
				FVector dir = (p_Player->GetActorLocation() - controlledActor->GetActorLocation()).GetSafeNormal();
				controlledActor->TryFireBullet(FVector2D(dir.X, dir.Y));
			}
		}
	}
	if (!controlledActor->TargetPosExist())
	{
		m_TimeSincePathFound = MAX_FLT;
	}
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
