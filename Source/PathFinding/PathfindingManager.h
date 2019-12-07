// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pathfinding.h"
#include "GameFramework/Actor.h"
#include "PathfindingManager.generated.h"

class Node
{
public:
	Node(int x, int y)
	{
		xPos = x;
		yPos = y;
	}
	Node(FVector2D gridPos)
	{
		xPos = gridPos.X;
		yPos = gridPos.Y;
	}
	Node()
	{
		xPos = -1;
		yPos = -1;
	}
	int getXPos() { return xPos; };
	int getYPos() { return yPos; };

	bool isWall() { return isObstructed; }
	void setIsWall(bool isWall)
	{
		isObstructed = isWall;
	}
	float dist = MAX_FLT;
	Node* previous = nullptr;

	FVector2D getGridPos() { return FVector2D(xPos, yPos); }
protected:
	int xPos;
	int yPos;
	bool isObstructed;
	
};

UCLASS()
class PATHFINDING_API APathfindingManager : public AActor
{
	GENERATED_BODY()
	
public:	
	APathfindingManager();

	int wallCount = 30;
	UFUNCTION(BlueprintCallable)
	void generate();
	UFUNCTION(BlueprintCallable)
	void generateWalls();

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> m_WallPositions;

	UFUNCTION(BlueprintCallable)
	TArray<FVector> getPathFromToDjikstra(FVector from, FVector to);
	UFUNCTION(BlueprintCallable)
	int getXHeight() { return xHeight; }
	UFUNCTION(BlueprintCallable)
	int getYHeight() { return yHeight; }
	UFUNCTION(BlueprintCallable)
	bool isNodeWall(int x, int y) { return nodes[x][y].isWall(); }

	FVector getWorldPositionFromGridPosition(Node* node);
	FVector getWorldPositionFromGridPosition(FVector2D pos);
	UFUNCTION(BlueprintCallable)
	FVector getWorldPositionFromGridPosition(int x, int y) { return getWorldPositionFromGridPosition(FVector2D(x, y)); }

	FVector2D getGridPositionFromWorldPosition(FVector pos);
	Node& getNodeFromWorldPosition(FVector pos);
	UFUNCTION(BlueprintCallable)
	void DebugDraw();

	void DebugDrawPath(TArray<FVector2D> path);
	int findBestCandidateInTArray(TArray<Node*>& nodes);
	int findENodeInTArray(TArray<Node*>& arr, Node* node);
	void refreshNodeGrid();
	UFUNCTION(BlueprintCallable)
	void generateWithWalls();
	TArray<FVector> convertGridPathToWorldPath(TArray<FVector2D> path);


	TArray<FVector2D> smoothPath(TArray<FVector2D>);
	bool checkIfClearBetweenPoints(FVector2D a, FVector2D b, float size);
	TArray<FVector2D> getNodeNeighbors(Node* node);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> wallClass;

	TArray<AActor*> Walls;
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FBox2D> m_ProhibitedSpawns;
	static const int xHeight = 40;
	static const int yHeight = 40;
	Node nodes[xHeight][yHeight];

	float nodeSize = 3000.0f/40.0f;
	float seekerRadius = 25.0f;
protected:
	virtual void BeginPlay() override;
};
