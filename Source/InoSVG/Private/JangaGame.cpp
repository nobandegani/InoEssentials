// InoLab Trials © 2024 Inoland Studio. All rights reserved.


#include "JangaGame.h"

UJangaGame::UJangaGame()
{
	// Set default block scale (e.g., 1 unit x 3 units x 1 unit)
	BlockScale = FVector(1.0f, 3.0f, 1.0f);
}

void UJangaGame::SetupGame(int32 MaxRows)
{
	Blocks.Empty();

	// Each row has 3 blocks
	int32 BlocksPerRow = 3;
	FVector StartPosition(0.0f, 0.0f, 0.0f);

	for (int32 Row = 0; Row < MaxRows; ++Row)
	{
		for (int32 i = 0; i < BlocksPerRow; ++i)
		{
			FBlock NewBlock;

			// Alternate rotation every row
			if (Row % 2 == 0)
			{
				NewBlock.Rotation = FRotator(0.0f, 0.0f, 0.0f);
				NewBlock.Location = StartPosition + FVector(i * BlockScale.X, 0.0f, Row * BlockScale.Z);
			}
			else
			{
				NewBlock.Rotation = FRotator(0.0f, 90.0f, 0.0f);
				NewBlock.Location = StartPosition + FVector(0.0f, i * BlockScale.Y, Row * BlockScale.Z);
			}

			Blocks.Add(NewBlock);
		}
	}
}

void UJangaGame::UpdateBlock(int32 Index, const FBlock& NewBlockData)
{
	if (Blocks.IsValidIndex(Index))
	{
		Blocks[Index] = NewBlockData;
		
		// After updating a block, simulate physics to adjust other blocks
		SimulatePhysics(0.016f); // Assuming a frame time of ~16ms
	}
}

void UJangaGame::SimulatePhysics(float DeltaTime)
{
	// Simple gravity
	FVector Gravity(0.0f, 0.0f, -980.0f); // Unreal units per second squared

	// Store previous locations and rotations before physics update
	for (FBlock& Block : Blocks)
	{
		Block.PreviousLocation = Block.Location;
		Block.PreviousRotation = Block.Rotation;
	}

	// Update velocities and positions due to gravity
	for (FBlock& Block : Blocks)
	{
		// Update velocity
		Block.Velocity += Gravity * DeltaTime;

		// Update position
		Block.Location += Block.Velocity * DeltaTime;

		// For this simple example, rotation isn't affected by physics
		// Update Block.Rotation if necessary in your simulation
	}

	// Resolve collisions between blocks
	ResolveCollisions();

	// After physics simulation, check for changes and broadcast events
	for (int32 i = 0; i < Blocks.Num(); ++i)
	{
		FBlock& Block = Blocks[i];

		bool bLocationChanged = !Block.Location.Equals(Block.PreviousLocation, KINDA_SMALL_NUMBER);
		bool bRotationChanged = !Block.Rotation.Equals(Block.PreviousRotation, KINDA_SMALL_NUMBER);

		if (bLocationChanged || bRotationChanged)
		{
			// Broadcast the event dispatcher
			OnBlockUpdated.Broadcast(i, Block.Location, Block.Rotation);
		}
	}
}

void UJangaGame::ResolveCollisions()
{
	for (int32 i = 0; i < Blocks.Num(); ++i)
	{
		for (int32 j = i + 1; j < Blocks.Num(); ++j)
		{
			if (IsColliding(Blocks[i], Blocks[j]))
			{
				// Simple collision response: stop movement
				Blocks[i].Velocity = FVector::ZeroVector;
				Blocks[j].Velocity = FVector::ZeroVector;

				// Adjust positions to prevent overlap
				FVector Overlap = (Blocks[i].Location - Blocks[j].Location);
				Overlap.Z = 0.0f; // Only adjust in X and Y
				Blocks[i].Location += Overlap * 0.5f;
				Blocks[j].Location -= Overlap * 0.5f;
			}
		}

		// Prevent blocks from falling below the ground
		if (Blocks[i].Location.Z < 0.0f)
		{
			Blocks[i].Location.Z = 0.0f;
			Blocks[i].Velocity.Z = 0.0f;
		}
	}
}

bool UJangaGame::IsColliding(const FBlock& A, const FBlock& B)
{
	// Simple AABB collision detection
	FVector AExtent = BlockScale * 0.5f;
	FVector BExtent = BlockScale * 0.5f;

	FVector AMin = A.Location - AExtent;
	FVector AMax = A.Location + AExtent;

	FVector BMin = B.Location - BExtent;
	FVector BMax = B.Location + BExtent;

	return (AMin.X <= BMax.X && AMax.X >= BMin.X) &&
		   (AMin.Y <= BMax.Y && AMax.Y >= BMin.Y) &&
		   (AMin.Z <= BMax.Z && AMax.Z >= BMin.Z);
}
