/* Copyright (c) 2021-2022 by Inoland */

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "Algo/DAStar.h"

#include "O_AStar.generated.h"

UENUM(BlueprintType, Category = "Ground Direction")
enum class EGroundDirection : uint8 {
	Manhattan = 0,
	Euclidean = 1,
	Octagonal = 2
};

UCLASS(Blueprintable, BlueprintType, meta = (DisplayName = "A* Algorithm", Keywords = "A*, Algorithm") , Category = "A* Algorithm")
class INOESSENTIALS_API UAStar : public UObject
{
	GENERATED_BODY()

protected:
	AStar::Generator generator;
	
public:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, meta = (DisplayName = "Row", Keywords = "") , Category = "A* Algorithm")
		int Row;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, meta = (DisplayName = "Col", Keywords = "") , Category = "A* Algorithm")
		int Col;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set World Size", Keywords = "") , Category = "A* Algorithm")
		void SetWorldSize(int X, int Y);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Diagonal Movement", Keywords = "") , Category = "A* Algorithm")
		void SetDiagonalMovement(bool IsDiagonal = true);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Heuristic", Keywords = "") , Category = "A* Algorithm")
		void SetHeuristic(EGroundDirection HeuristicMode = EGroundDirection::Euclidean);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add Collisions", Keywords = "") , Category = "A* Algorithm")
		void AddCollisions(TArray<FVector2D> Collisions, bool bClearCollisions = true);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Remove Collisions", Keywords = "") , Category = "A* Algorithm")
		void RemoveCollisions(TArray<FVector2D> Collisions);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Clear Collisions", Keywords = "") , Category = "A* Algorithm")
		void ClearCollisions();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Find Path", Keywords = "") , Category = "A* Algorithm")
		void FindPath(FVector2D Source, FVector2D Destination, TArray<FVector2D>& Path);
};