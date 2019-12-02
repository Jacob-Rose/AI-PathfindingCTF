// Fill out your copyright notice in the Description page of Project Settings.


#include "CTF_AICharacter.h"
#include "EngineUtils.h"
#include "CTFStateMachine.h"
#include "PathfindingManager.h"

// Sets default values
ACTF_AICharacter::ACTF_AICharacter()
{
 	
}

// Called when the game starts or when spawned
void ACTF_AICharacter::BeginPlay()
{
	m_StateMachine = NewObject<UCTFStateMachine>();
	Super::BeginPlay();
	m_StateMachine->p_Flag = p_Flag;
	m_StateMachine->p_MyBase = p_MyBase;
	m_StateMachine->p_PlayerBase = p_PlayerBase;
}

// Called every frame
void ACTF_AICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_StateMachine->HandleState(this, DeltaTime);
}

bool ACTF_AICharacter::HasFlag()
{
	TArray<AActor*> attached = TArray<AActor*>();
	GetAttachedActors(attached);
	return attached.Num() > 0;
}


