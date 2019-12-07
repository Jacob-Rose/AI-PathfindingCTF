// Fill out your copyright notice in the Description page of Project Settings.


#include "CTF_Character.h"
#include <Engine/World.h>
#include <Runtime\Engine\Classes\Kismet\KismetMathLibrary.h>

TArray<AActor*> ACTF_Character::sm_Bullets = TArray<AActor*>();

// Sets default values
ACTF_Character::ACTF_Character()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACTF_Character::BeginPlay()
{
	Super::BeginPlay();
	Position = GetActorLocation();
	
}

// Called every frame
void ACTF_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Position = GetActorLocation();
	Position += (GetSteeringVelocity() * DeltaTime);
	SetActorLocation(Position, true);
	Position = GetActorLocation();

	// orientation 
	FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(Position, Position + GetSteeringVelocity());
	SetActorRotation(PlayerRot);
}

bool ACTF_Character::TryFireBullet(FVector2D dirVec)
{
	FVector pos = GetActorLocation() + (FVector(dirVec.X, dirVec.Y, 0) * Radius);

	FRotator dir = FVector(dirVec.X, dirVec.Y, 0).Rotation();
	if (m_CurrentBullet == nullptr || !m_CurrentBullet->IsValidLowLevel() || !IsValid(m_CurrentBullet))
	{
		m_CurrentBullet = GetWorld()->SpawnActor(bulletType, &pos, &dir);
		sm_Bullets.Add(m_CurrentBullet);
		return true;
	}
	else
	{
		return false;
	}
}

void ACTF_Character::TryDestroyBullet()
{
	if (m_CurrentBullet != nullptr || m_CurrentBullet->IsValidLowLevel() || IsValid(m_CurrentBullet))
	{
		m_CurrentBullet->Destroy();
	}
}

void ACTF_Character::SetSteeringVelocity(FVector sv)
{
	SteeringVelocity = FVector2D(sv.X, sv.Y);
	if (SteeringVelocity.Size() > m_MaxSpeed)
	{
		SteeringVelocity = SteeringVelocity.GetSafeNormal() * m_MaxSpeed;
	}
}

FVector ACTF_Character::GetSteeringVelocity()
{
	return FVector(SteeringVelocity.X, SteeringVelocity.Y, 0);
}

