// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PathFindingActor.h"
#include "CTF_AICharacter.generated.h"

class APathfindingManager;
class UCTFStateMachine;

UCLASS()
class PATHFINDING_API ACTF_AICharacter : public APathFindingActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACTF_AICharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		AActor* p_PlayerBase = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		AActor* p_MyBase = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		AActor* p_Flag = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float m_MaxShootDist = 500.0f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	APathfindingManager* pathfinder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCTFStateMachine* m_StateMachine;
};
