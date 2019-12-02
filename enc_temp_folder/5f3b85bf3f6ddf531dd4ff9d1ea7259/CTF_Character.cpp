// Fill out your copyright notice in the Description page of Project Settings.


#include "CTF_Character.h"
#include <Runtime\Engine\Classes\Kismet\KismetMathLibrary.h>

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
	Position += (SteeringVelocity * DeltaTime);
	SetActorLocation(Position, true);
	Position = GetActorLocation();

	// orientation 
	//FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(Position, Position + SteeringVelocity);
	//SetActorRotation(PlayerRot);
}

