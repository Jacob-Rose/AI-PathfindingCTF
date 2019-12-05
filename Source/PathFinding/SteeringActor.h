// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTF_Character.h"
#include "SteeringActor.generated.h"

UCLASS()
class PATHFINDING_API ASteeringActor : public ACTF_Character
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASteeringActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintCallable)
	void Seek();
	FVector Avoid();
	FVector AvoidBullet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector targetDir = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering")
	float MinSpeed = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering")
	float DragForce = -0.8f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering")
	float SeekStrength = 900.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering")
	float AvoidStrength = 900.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering")
	float SeekDecelerationDistance = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetPos = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering")
	float Radius = 25.0f;
};
