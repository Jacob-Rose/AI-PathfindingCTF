// Fill out your copyright notice in the Description page of Project Settings.


#include "Seeker.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "EngineUtils.h"

// Sets default values
ASeeker::ASeeker()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASeeker::BeginPlay()
{
	m_StateMachine = NewObject<USeekerStateMachine>();
	for (TActorIterator<APathfindingManager> It(GetWorld()); It; ++It)
	{
		pathfinder = *It; //just use the first one
		break;
	}
	Super::BeginPlay();
}

// Called every frame
void ASeeker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_StateMachine->HandleState(this, DeltaTime);
}


