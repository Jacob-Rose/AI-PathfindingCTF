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
	static TArray<AActor*> sm_Bullets;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//returns if shot
	UFUNCTION(BlueprintCallable)
	bool TryFireBullet(FVector2D dir);

	UFUNCTION(BlueprintCallable)
	void TryDestroyBullet();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> bulletType;

	UFUNCTION(BlueprintCallable)
	void SetSteeringVelocity(FVector sv);
	UFUNCTION(BlueprintCallable)
	FVector GetSteeringVelocity();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasFlag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CharacterJustMoved = false;
protected:
	FVector Position = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_MaxSpeed;
	UPROPERTY(BlueprintReadWrite)
	FVector2D SteeringVelocity = FVector2D(0,0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 25.0f;

	AActor* m_CurrentBullet;
};
