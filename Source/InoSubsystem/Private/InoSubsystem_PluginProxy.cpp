/* Copyright (c) 2021-2024 by Inoland */


#include "InoSubsystem_PluginProxy.h"
#include "InoSubsystemFunctions.h"
#include "Engine/AssetManager.h"

void UInoSubsystem_PluginProxy::Initialize(FSubsystemCollectionBase& Collection)
{
	// UAssetManager::Get();
	
	TSet<FString> PreparedPaths;
	FIND_AllPluginsDirectory(PreparedPaths);
	LOAD_ByPaths_InoSubsystem(PreparedPaths);
}
