// Fill out your copyright notice in the Description page of Project Settings.


#include "CTF_AICharacter.h"
#include "EngineUtils.h"
#include "CTFStateMachine.h"
#include "PathfindingManager.h"
#include <Runtime\Engine\Classes\Kismet\KismetMathLibrary.h>

// Sets default values
ACTF_AICharacter::ACTF_AICharacter()
{
 	
}

// Called when the game starts or when spawned
void ACTF_AICharacter::BeginPlay()
{
	m_StateMachine = NewObject<UCTFStateMachine>();
	m_StateMachine->p_Flag = p_Flag;
	m_StateMachine->p_MyBase = p_MyBase;
	m_StateMachine->p_PlayerBase = p_PlayerBase;
	Super::BeginPlay();
}

// Called every frame
void ACTF_AICharacter::Tick(float DeltaTime)
{
	AvoidBullet();
	Super::Tick(DeltaTime);
	m_StateMachine->HandleState(this, DeltaTime);
}




