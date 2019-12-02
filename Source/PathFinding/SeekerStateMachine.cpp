#include "SeekerStateMachine.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Seeker.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"

UState::UState()
{
}

UState::~UState()
{
}

void UPatrolState::RunState(ASeeker* seeker, float deltaTime)
{
	TArray<FVector> cPath = seeker->GetPath();
	if (m_HasReturnedToLoop)
	{
		if (cPath.Num() < m_Positions.Num())
		{
			TArray<FVector> toAdd;
			for (int i = 0; i < m_Positions.Num(); i++)
			{
				if (!cPath.Contains(m_Positions[i]))
				{
					toAdd.Add(m_Positions[i]);
				}
			}
			seeker->AddToPathFollow(toAdd);
		}
	}
	else
	{
		if (seeker->NeedNewDestination)
		{
			m_HasReturnedToLoop = true;
		}
	}
	
}
void UPatrolState::SetTarget(APawn* target)
{
	m_Target = target;
}
USeekerStateMachine::USeekerStateMachine()
{
	
}

bool UPatrolState::ShouldPursueTarget(ASeeker* seeker)
{
	FVector distanceVec = (m_Target->GetActorLocation()- seeker->GetActorLocation());
	FVector forwardVec = seeker->GetActorForwardVector().GetSafeNormal();
	if (distanceVec.SizeSquared() < m_SeeingDistance * m_SeeingDistance)
	{
		distanceVec = distanceVec.GetSafeNormal();
		float aDotB = FVector::DotProduct(forwardVec, distanceVec);
		//float theta = FMath::Acos((aDotB * aDotB) / (forwardVec.SizeSquared() * distanceVec.SizeSquared()));
		float theta = (FMath::Acos(aDotB))*(180/PI);
		
		if (FMath::Abs(theta) < m_FieldOfViewAngle)
		{
			return true;
		}
	}
	return false;
}
UPursuitState::UPursuitState()
{
	
}

void UPursuitState::RunState(ASeeker* seeker, float deltaTime)
{
	FHitResult result;
	FCollisionQueryParams cqp;
	cqp.AddIgnoredActor(seeker);
	if (FVector::DistSquared(m_Target->GetActorLocation(), seeker->GetActorLocation()) <= m_FleeingDistance * m_FleeingDistance &&
		!seeker->GetWorld()->LineTraceSingleByObjectType(result, seeker->GetActorLocation(),
		m_Target->GetActorLocation(), 
		FCollisionObjectQueryParams::AllObjects, cqp))
		
	{
		m_TimeSinceSeenPlayer = 0.0f;
		m_LastTargetPos = m_Target->GetActorLocation();

		if (timeSincePathFound > 1.0f) //add check if player position and last path node distance greater than x
		{
			TArray<FVector> newPath = pathfinder->getPathFromToDjikstra(seeker->GetActorLocation(), m_LastTargetPos);
			seeker->SetPathToFollow(newPath);
			timeSincePathFound = 0.0f;
		}
		timeSincePathFound += deltaTime;
	}
	else
	{
		m_TimeSinceSeenPlayer += deltaTime;
	}
}

bool UPursuitState::ShouldExitPursuit()
{
	if (m_TimeSinceSeenPlayer > m_MaxTimeBeforeQuit)
	{
		return true;
	}
	return false;
}

void UPursuitState::SetTarget(APawn* target)
{
	m_Target = target;
}

void UPursuitState::SetPathfinder(APathfindingManager* pathfinder)
{
	this->pathfinder = pathfinder;
}

void USeekerStateMachine::HandleState(ASeeker* seeker, float deltaTime)
{
	if (m_IsInit)
	{
		m_CurrentState->RunState(seeker, deltaTime);
		if (UPursuitState* state = dynamic_cast<UPursuitState*>(m_CurrentState))
		{
			if (state->ShouldExitPursuit())
			{
				for (int i = 0; i < m_States.Num(); i++) //this seems not proper
				{
					if (UPatrolState* pState = dynamic_cast<UPatrolState*>(m_States[i]))
					{
						ChangeToState(pState);
					}
				}
			}
		}
		else if (UPatrolState* state = dynamic_cast<UPatrolState*>(m_CurrentState))
		{
			if (state->ShouldPursueTarget(seeker))
			{
				for (int i = 0; i < m_States.Num(); i++) //this seems not proper
				{
					if (UPursuitState* pState = dynamic_cast<UPursuitState*>(m_States[i]))
					{
						ChangeToState(pState);
					}
				}
			}
		}
	}
	
}

void USeekerStateMachine::init(APawn* player)
{
	if (!m_IsInit)
	{
		m_Target = player;
		if (player == nullptr)
		{
			throw "oohwee thats a zoinks";
		}
		APathfindingManager* pathfinder = nullptr;
		for (TActorIterator<APathfindingManager> it(player->GetWorld()); it; ++it)
		{
			pathfinder = *it;
			break;
		}
		check(IsValid(pathfinder));
		UPatrolState* patrolState = NewObject<UPatrolState>();
		UPursuitState* pursuitState = NewObject<UPursuitState>();
		pursuitState->SetTarget(m_Target);
		pursuitState->SetPathfinder(pathfinder);
		patrolState->SetTarget(m_Target);

		patrolState->m_Positions.Add(pathfinder->getWorldPositionFromGridPosition(FVector2D(11,11)));
		patrolState->m_Positions.Add(pathfinder->getWorldPositionFromGridPosition(FVector2D(13,11)));
		patrolState->m_Positions.Add(pathfinder->getWorldPositionFromGridPosition(FVector2D(13,13)));
		patrolState->m_Positions.Add(pathfinder->getWorldPositionFromGridPosition(FVector2D(11,13)));

		m_States.Add(patrolState);
		m_States.Add(pursuitState);

		ChangeToState(m_States[0]);
		m_IsInit = true;
	}
}

void USeekerStateMachine::ChangeToState(UState* state)
{
	if (state != m_CurrentState)
	{
		m_CurrentState = state;
	}
}