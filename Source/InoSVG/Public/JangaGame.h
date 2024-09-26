// InoLab Trials © 2024 Inoland Studio. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "JangaGame.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnBlockUpdated, int32, Index, FVector, Location, FRotator, Rotation);

USTRUCT(BlueprintType)
struct FBlock
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Block")
	FVector Location;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Block")
	FRotator Rotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Block")
	FVector Velocity;

	// Add previous location and rotation to track changes
	FVector PreviousLocation;
	FRotator PreviousRotation;

	FBlock()
		: Location(FVector::ZeroVector),
		  Rotation(FRotator::ZeroRotator),
		  Velocity(FVector::ZeroVector),
		  PreviousLocation(FVector::ZeroVector),
		  PreviousRotation(FRotator::ZeroRotator)
	{
	}
};

/**
 * 
 */
UCLASS(Blueprintable)
class INOSVG_API UJangaGame : public UObject
{
	GENERATED_BODY()
	
public:
	UJangaGame();

	// Block size
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Jenga")
	FVector BlockScale;

	// Array of all blocks
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Jenga")
	TArray<FBlock> Blocks;

	// Initialize the game with a certain number of rows
	UFUNCTION(BlueprintCallable, Category = "Jenga")
	void SetupGame(int32 MaxRows);

	// Update a specific block
	UFUNCTION(BlueprintCallable, Category = "Jenga")
	void UpdateBlock(int32 Index, const FBlock& NewBlockData);

	// Simulate physics
	UFUNCTION(BlueprintCallable, Category = "Jenga")
	void SimulatePhysics(float DeltaTime);

	// Event dispatcher for when a block is updated
	UPROPERTY(BlueprintAssignable, Category = "Jenga")
	FOnBlockUpdated OnBlockUpdated;

private:
	// Helper functions for collision detection and resolution
	void ResolveCollisions();
	bool IsColliding(const FBlock& A, const FBlock& B);
};
