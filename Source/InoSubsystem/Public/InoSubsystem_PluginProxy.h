/* Copyright (c) 2021-2024 by Inoland */

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "InoSubsystem_PluginProxy.generated.h"

/**
 * 
 */
UCLASS()
class INOSUBSYSTEM_API UInoSubsystem_PluginProxy : public UEngineSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
