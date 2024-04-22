/* Copyright (c) 2021-2024 by Inoland */

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
/**
 * 
 */
class FTickableSubsystem : public FTickableGameObject
{
public:
	FTickableSubsystem()
		: FTickableGameObject(),
		  bIsTickEnabled(false),
		  bIsTickableWhenPaused(false)
	{
	}

private:
	uint8 bIsTickEnabled:1;
	uint8 bIsTickableWhenPaused:1;

public:
	virtual TStatId GetStatId() const override =0;
	virtual bool IsTickableInEditor() const override final { return false; }

	virtual void Tick(float DeltaSeconds) override =0;

	virtual bool IsTickable() const override { return bIsTickEnabled; }
	virtual bool IsTickableWhenPaused() const override { return bIsTickableWhenPaused; }
public:
	virtual void SetTickEnabled(bool const bEnabled) { bIsTickEnabled = bEnabled; }
	virtual void SetTickableWhenPaused(bool const bTickableWhenPaused) { bIsTickableWhenPaused = bTickableWhenPaused; }
};
