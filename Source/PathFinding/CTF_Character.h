// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CTF_Character.generated.h"

UCLASS()
class PATHFINDING_API ACTF_Character : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACTF_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_MaxSpeed;
	UFUNCTION(BlueprintCallable)
	void TryFireBullet(FRotator dir);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> bulletType;

	bool HasFlag();


protected:

	FVector Position = FVector(0, 0, 0);
	UPROPERTY(BlueprintReadWrite)
	FVector SteeringVelocity = FVector(0,0,0);

	AActor* m_CurrentBullet;

	int m_Score;
};
