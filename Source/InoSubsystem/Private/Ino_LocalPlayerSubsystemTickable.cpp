/* Copyright (c) 2021-2024 by Inoland */

#include "Ino_LocalPlayerSubsystemTickable.h"
#include "InoSubsystem/InoSubsystemTickable.h"

void UIno_LocalPlayerSubsystemTickable::Tick(float DeltaSeconds)
{
	checkf(bInitialized,
	       TEXT(
		       "Ticking should have been disabled for an uninitialized subsystem : remember to call IsInitialized in the subsystem's IsTickable, IsTickableInEditor and/or IsTickableWhenPaused implementation"
	       ));
	ReceiveTick(DeltaSeconds);
}

ETickableTickType UIno_LocalPlayerSubsystemTickable::GetTickableTickType() const
{
	return IsTemplate() ? ETickableTickType::Never : FTickableSubsystem::GetTickableTickType();
}

bool UIno_LocalPlayerSubsystemTickable::IsTickable() const
{
	return FTickableSubsystem::IsTickable() && !IsTemplate() && bInitialized && IsActive();
}

void UIno_LocalPlayerSubsystemTickable::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SetTickEnabled(bIsTickEnabled);
	SetTickableWhenPaused(bIsTickableWhenPaused);
}
