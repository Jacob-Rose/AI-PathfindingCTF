// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingManager.h"
#include <cmath>
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


// Sets default values
APathfindingManager::APathfindingManager()
{
	
}


void APathfindingManager::generate()
{
	for (int i = 0; i < xHeight; i++)
	{
		for (int j = 0; j < yHeight; j++)
		{
			nodes[i][j] = Node(i, j);
			nodes[i][j].dist = MAX_FLT;
			nodes[i][j].setIsWall(false);
			//instantiate physical
		}
	}
}

void APathfindingManager::generateWalls()
{
	int eCheck = 0;
	for (int i = 0; i < wallCount; i++)
	{
		int xPos = rand() % xHeight;
		int yPos = rand() % yHeight;
		if (!nodes[xPos][yPos].isWall()) //he patrols in 10 < x < 14 && 10 < y < 14
		{
			FVector pos = getWorldPositionFromGridPosition(nodes[xPos][yPos].getGridPos());
			FVector2D pos2D = FVector2D(pos.X, pos.Y);
			bool valid = true;
			for (int i = 0; i < m_ProhibitedSpawns.Num(); i++)
			{
				float relX = FMath::Abs((pos2D.X - m_ProhibitedSpawns[i].GetCenter().X));
				float relY = FMath::Abs((pos2D.Y - m_ProhibitedSpawns[i].GetCenter().Y));
				float extX = m_ProhibitedSpawns[i].GetExtent().X;
				float extY = m_ProhibitedSpawns[i].GetExtent().Y;
				if (relX < extX &&
					relY < extY)
				{
					i--;
					eCheck++;
					valid = false;
					break;
				}
			}
			if (valid)
			{
				nodes[xPos][yPos].setIsWall(true);

				m_WallPositions.Add(pos);

				AActor* wall = GetWorld()->SpawnActor(wallClass, &pos, &FRotator::ZeroRotator);
			}
			
		}
		else
		{
			i--;
			eCheck++;
		}
		if (eCheck > 25)
		{
			print("not all walls spawned");
			break;
		}
	}
}

FVector2D APathfindingManager::getGridPositionFromWorldPosition(FVector pos)
{
	return FVector2D(FMath::DivideAndRoundUp(pos.X, nodeSize), FMath::DivideAndRoundUp(pos.Y, nodeSize));
}

Node& APathfindingManager::getNodeFromWorldPosition(FVector pos)
{
	// TODO: insert return statement here
	FVector2D gridPos = FVector2D(pos.X / nodeSize, pos.Y / nodeSize);
	return nodes[lroundf( gridPos.X)][lroundf(gridPos.Y)];
}

void APathfindingManager::DebugDraw()
{
	for (int i = 0; i < xHeight; i++)
	{
		for (int j = 0; j < yHeight; j++)
		{
			FVector pos = getWorldPositionFromGridPosition(&nodes[i][j]);
			FColor c;
			if (nodes[i][j].isWall())
			{
				
				c = FColor::Yellow;
			}
			else
			{
				c = FColor::Red;
			}
			DrawDebugBox(GetWorld(), pos, FVector(nodeSize/4, nodeSize/4, nodeSize/4), c, true);
		}
	}
}

int APathfindingManager::findBestCandidateInTArray(TArray<Node*>& nodes)
{
	int currentBest = -1;
	float distance = MAX_FLT;
	for (int i = 0; i < nodes.Num(); ++i)
	{
		if (distance > nodes[i]->dist)
		{
			currentBest = i;
			distance = nodes[i]->dist;
		}
	}
	if (currentBest == -1)
	{
		currentBest = 0;
	}
	return currentBest;
}

int APathfindingManager::findENodeInTArray(TArray<Node*>& arr, Node* node)
{
	for (int i = 0; i < arr.Num(); i++)
	{
		if (arr[i] == node)
		{
			return i;
		}
	}
	return -1;
}

void APathfindingManager::refreshNodeGrid()
{
	for (int i = 0; i < xHeight; i++)
	{
		for (int j = 0; j < yHeight; j++)
		{
			nodes[i][j].dist = MAX_FLT;
		}
	}
}

void APathfindingManager::generateWithWalls()
{
	generate();
	generateWalls();
}

TArray<FVector> APathfindingManager::convertGridPathToWorldPath(TArray<FVector2D> path)
{
	TArray<FVector> newPath;
	for (int i = 0; i < path.Num(); i++)
	{
		newPath.Add(getWorldPositionFromGridPosition(path[i]));
	}
	return newPath;
}

TArray<FVector2D> APathfindingManager::smoothPath(TArray<FVector2D> path)
{
	TArray<FVector2D> newPath;
	newPath.Add(path[0]);
	for (int i = 0; i < path.Num(); i++)
	{
		for (int j = i + 1; j < path.Num(); j++)
		{
			if (!checkIfClearBetweenPoints(path[i], path[j], seekerRadius)) //TODO Radius is not defined in code
			{
				newPath.Add(path[j-1]);
				i = j-1;
				break;
			}
		}
	}
	newPath.Add(path[path.Num() - 1]); //add the last node in
	return newPath;
}

bool APathfindingManager::checkIfClearBetweenPoints(FVector2D a, FVector2D b, float radius)
{
	//Just a raycast, no account for size of the player
	FHitResult resultLft;
	FHitResult resultRgt;
	FVector perpNorm = FVector((a - b).Y, -(a - b).X, 0).GetSafeNormal();
	if (GetWorld()->LineTraceSingleByObjectType(resultLft, getWorldPositionFromGridPosition(a) + (perpNorm * radius),
		getWorldPositionFromGridPosition(b) + (perpNorm * radius), FCollisionObjectQueryParams::AllObjects)
		&& GetWorld()->LineTraceSingleByObjectType(resultRgt, getWorldPositionFromGridPosition(a) - (perpNorm * radius),
			getWorldPositionFromGridPosition(b) - (perpNorm * radius), FCollisionObjectQueryParams::AllObjects))
	{
		return false;
	}
	return true;
}

TArray<FVector2D> APathfindingManager::getNodeNeighbors(Node* node)
{
	TArray<FVector2D> myNodes;
	if (node->getXPos() > 1)
	{
		myNodes.Add(FVector2D(node->getXPos() - 1, node->getYPos()));
	}
	if (node->getXPos() < xHeight-1)
	{
		myNodes.Add(FVector2D(node->getXPos() + 1, node->getYPos()));
	}
	if (node->getYPos() > 1)
	{
		myNodes.Add(FVector2D(node->getXPos(), node->getYPos() - 1));
	}
	if (node->getYPos() < yHeight - 1)
	{
		myNodes.Add(FVector2D(node->getXPos(), node->getYPos() + 1));
	}
	return myNodes;
}

void APathfindingManager::BeginPlay()
{
	generate();
	generateWalls();
	//spawnwalls
}

TArray<FVector> APathfindingManager::getPathFromToDjikstra(FVector from, FVector to)
{

	Node* fromNode = &getNodeFromWorldPosition(from);
	Node* endNode = &getNodeFromWorldPosition(to);
	TArray<Node*> open;
	open.Add(fromNode);
	fromNode->dist = 0;
	TArray<Node*> closed;
	while (open.Num() > 0)
	{
		//select the next best candidate (search for best)djikstra uses distance between the node position and the from node
		int index = findBestCandidateInTArray(open);//=findBestIndex();
		//remove the current from the open list
		Node* current = open[index];
		open.RemoveAt(index);
		TArray<FVector2D> neighbors = getNodeNeighbors(current);
		if (current == endNode) //they are the same memory 
		{
			TArray<FVector2D> path;
			//go through and add each, starting from end
			Node* tmp = current;
			
			while (tmp != fromNode)
			{
				path.Add(tmp->getGridPos());
				tmp = tmp->previous;
			}
			path.Add(fromNode->getGridPos());
			Algo::Reverse(path);
			
			path = smoothPath(path);
			//path.RemoveAt(0); //once smoothed, remove the start node (he is already there)
			refreshNodeGrid();

			//DEBUG DRAW
			DebugDrawPath(path);
			//flip path
			return convertGridPathToWorldPath(path);
		}
		//get neighbors of best node
		//for each neighbor, if the distance from neighbor is greater than the current distance, add to open list
		for (int i = 0; i < neighbors.Num(); i++)
		{
			float distMultiplier = (neighbors[i] - FVector2D(getWorldPositionFromGridPosition(current->getGridPos()))).Size() / nodeSize;
			float tmpDist = current->dist + nodeSize * distMultiplier;//todo
			Node& nNode = nodes[(int)neighbors[i].X][(int)neighbors[i].Y];
			if (!nNode.isWall() && nNode.dist > tmpDist )
			{
				nodes[(int)neighbors[i].X][(int)neighbors[i].Y].previous = current;
				nodes[(int)neighbors[i].X][(int)neighbors[i].Y].dist = tmpDist;
				open.Add(&nodes[(int)neighbors[i].X][(int)neighbors[i].Y]);
			}
		}
	}
	return TArray<FVector>();
}

void APathfindingManager::DebugDrawPath(TArray<FVector2D> path)
{
	if (path.Num() > 1)
	{
		DrawDebugSphere(GetWorld(), getWorldPositionFromGridPosition(path[0]), 20.0f, 12, FColor::Magenta, false, 6.0f, 0);
		for (int i = 1; i < path.Num(); i++)
		{
			DrawDebugLine(
				GetWorld(),
				getWorldPositionFromGridPosition(path[i - 1]),
				getWorldPositionFromGridPosition(path[i]),
				FColor(255, 255, 255),
				false, // sets weather or not the line is in the world permanently
				6.0f, 0,
				5
			);

			DrawDebugSphere(GetWorld(), getWorldPositionFromGridPosition(path[i]), 20.0f, 12, FColor::Magenta, false, 6.0f, 0);
		}
	}
}


FVector APathfindingManager::getWorldPositionFromGridPosition(Node* node)
{
	return getWorldPositionFromGridPosition(node->getGridPos());
}

FVector APathfindingManager::getWorldPositionFromGridPosition(FVector2D pos)
{
	int boardXCoord = pos.X * nodeSize;
	int boardYCoord = pos.Y * nodeSize;
	return FVector(boardXCoord, boardYCoord, 0.0f);
}



