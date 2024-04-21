// © 2024. All rights reserved by Inoland

#include "Ino_EngineSubsystem.h"
#include "InoSubsystem/InoSubsystemTypes.h"

bool UIno_EngineSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && ReceiveShouldCreateSubsystem(Outer);
}

void UIno_EngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ReceiveInitialize();
	UE_LOG(LogInoSubsystem, Warning, TEXT("%s_Initialize"), *GetName())
}

void UIno_EngineSubsystem::Deinitialize()
{
	ReceiveDeinitialize();
	Super::Deinitialize();
	UE_LOG(LogInoSubsystem, Warning, TEXT("%s_Deinitialize"), *GetName())
}

bool UIno_EngineSubsystem::ReceiveShouldCreateSubsystem_Implementation(UObject* Outer) const
{
	return true;
}
