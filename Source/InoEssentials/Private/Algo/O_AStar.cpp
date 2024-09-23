/* Copyright (c) 2021-2023 by Inoland */

#include "Algo/O_AStar.h"


void UAStar::SetWorldSize(int InCol, int InRow)
{
	Col = InCol;
	Row = InRow;
	generator.setWorldSize({Row, Col});
}

void UAStar::SetHeuristic(EGroundDirection HeuristicMode)
{
	switch (HeuristicMode)
	{
	case EGroundDirection::Euclidean:
		generator.setHeuristic(AStar::Heuristic::euclidean);
		break; 
	case EGroundDirection::Manhattan:
		generator.setHeuristic(AStar::Heuristic::manhattan);
		break;
	case EGroundDirection::Octagonal:
		generator.setHeuristic(AStar::Heuristic::octagonal);
		break;
	default:
		generator.setHeuristic(AStar::Heuristic::euclidean);
		break; 
	}
}

void UAStar::SetDiagonalMovement(bool IsDiagonal)
{
	generator.setDiagonalMovement(IsDiagonal);
}

void UAStar::AddCollisions(TArray<FVector2D> Collisions, bool bClearCollisions)
{
	if (bClearCollisions)
	{
		ClearCollisions();
	}
	for(auto& ICollision : Collisions) {
		AStar::Vec2i DCollision;
		DCollision.x = ICollision.X;
		DCollision.y = ICollision.Y;
		generator.addCollision(DCollision);
	}
}

void UAStar::RemoveCollisions(TArray<FVector2D> Collisions)
{
	for (auto& ICollision : Collisions)
	{
		generator.removeCollision( AStar::Vec2i(ICollision.X, ICollision.Y) );
	};
}

void UAStar::ClearCollisions()
{
	generator.clearCollisions();
}

void UAStar::FindPath(FVector2D Source, FVector2D Destination, TArray<FVector2D>& Path)
{
	auto TempPath = generator.findPath(AStar::Vec2i(Source.X, Source.Y), AStar::Vec2i(Destination.X, Destination.Y));
	for(auto& Coordinate : TempPath) {
		FVector2D TempCoord;
		TempCoord.X = Coordinate.x;
		TempCoord.Y = Coordinate.y;
		Path.Add(TempCoord);
	}
}
