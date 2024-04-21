/* Copyright (c) 2021-2024 by Inoland */

#include "Ino_WorldSubsystemTickable.h"
#include "InoSubsystem/InoSubsystemTickable.h"

void UIno_WorldSubsystemTickable::Tick(float DeltaTime)
{
	checkf(bInitialized,
	       TEXT(
		       "Ticking should have been disabled for an uninitialized subsystem : remember to call IsInitialized in the subsystem's IsTickable, IsTickableInEditor and/or IsTickableWhenPaused implementation"
	       ));
	ReceiveTick(DeltaTime);
}

ETickableTickType UIno_WorldSubsystemTickable::GetTickableTickType() const
{
	return IsTemplate() ? ETickableTickType::Never : FTickableSubsystem::GetTickableTickType();
}

bool UIno_WorldSubsystemTickable::IsTickable() const
{
	return FTickableSubsystem::IsTickable() && !IsTemplate() && bInitialized && IsActive();
}

void UIno_WorldSubsystemTickable::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SetTickEnabled(bIsTickEnabled);
	SetTickableWhenPaused(bIsTickableWhenPaused);
}
