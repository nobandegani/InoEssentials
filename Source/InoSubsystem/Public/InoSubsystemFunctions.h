/* Copyright (c) 2021-2024 by Inoland */

#pragma once

#include "CoreMinimal.h"

#define LOAD_ByPaths_InoSubsystem(Paths) FInoSubsystemFunctions::LoadAllInoSubsystemByPaths(Paths)

#define FIND_AllPluginsDirectory(PluginDirectorys) FInoSubsystemFunctions::FindAllPluginsDirectory(PluginDirectorys)


class INOSUBSYSTEM_API FInoSubsystemFunctions
{
public:
	
	static void FindAllPluginsDirectory(TSet<FString>& PluginDirectorys);
	static void LoadSubsystemByPaths(UClass* SubsystemClass, const TSet<FString>& Paths);
	static void LoadAllInoSubsystemByPaths(const TSet<FString>& Paths);
};
