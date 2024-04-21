/* Copyright (c) 2021-2024 by Inoland */

#include "InoSubsystemFunctions.h"
#include "Ino_GameInstanceSubsystem.h"
#include "Ino_LocalPlayerSubsystem.h"
#include "Ino_WorldSubsystem.h"
#include "InoSubsystem/InoSubsystemTypes.h"
#include "Engine/ObjectLibrary.h"
#include "Engine/AssetManager.h"
#include "Interfaces/IPluginManager.h"

void FInoSubsystemFunctions::FindAllPluginsDirectory(TSet<FString>& PluginDirectorys)
{
	UE_LOG(LogInoSubsystem, Log, TEXT("Begin to scan InoSubsystem Asset"))
	IPluginManager& PluginManager = IPluginManager::Get();

	auto Plugins = PluginManager.GetEnabledPluginsWithContent();
	TSet<FString> PluginNames;

	for (auto& Plugin : Plugins)
	{
		if (Plugin->GetBaseDir().Contains("Marketplace") || Plugin->GetLoadedFrom() == EPluginLoadedFrom::Project)
		{
			PluginNames.Add(Plugin->GetMountedAssetPath());
			UE_LOG(LogInoSubsystem, Log, TEXT("Plugin %s Path: %s"), *Plugin->GetName(), *Plugin->GetMountedAssetPath())
		}
	}

	PluginDirectorys.Add("/Game/");
	PluginDirectorys.Add("/InoEssentials/");
	UE_LOG(LogInoSubsystem, Log, TEXT("End of scan InoSubsystem Asset"))
}

void FInoSubsystemFunctions::LoadSubsystemByPaths(UClass* SubsystemClass, const TSet<FString>& Paths)
{
	UE_LOG(LogInoSubsystem, Log, TEXT("Begin to load all of %s Asset"), *SubsystemClass->GetName())

	UObjectLibrary* ObjectLibrary = UObjectLibrary::CreateLibrary(SubsystemClass, true, GIsEditor);
	ObjectLibrary->AddToRoot();
	if (!IsValid(ObjectLibrary))
	{
		UE_LOG(LogInoSubsystem, Warning,
		       TEXT("Unable to create Ino_Subsystem ObjectLibrary. Cause the SubBlueprint of Ino_... cant be uesd"
		       ));
		return;
	}
	ObjectLibrary->LoadBlueprintAssetDataFromPaths(Paths.Array());
	ObjectLibrary->LoadAssetsFromAssetData();
	// ObjectLibrary->LoadBlueprintsFromPaths(Paths.Array());

	UE_LOG(LogInoSubsystem, Log, TEXT("End of load all of %s Asset"), *SubsystemClass->GetName())
}

void FInoSubsystemFunctions::LoadAllInoSubsystemByPaths(const TSet<FString>& Paths)
{
	UE_LOG(LogInoSubsystem, Log, TEXT("Begin to load all of InoSubsystem Asset"))
	LoadSubsystemByPaths(UIno_GameInstanceSubsystem::StaticClass(), Paths);
	LoadSubsystemByPaths(UIno_LocalPlayerSubsystem::StaticClass(), Paths);
	LoadSubsystemByPaths(UIno_WorldSubsystem::StaticClass(), Paths);

	// LoadSubsystemByPaths(UIno_EngineSubsystem::StaticClass(), Paths);
	// LoadSubsystemByPaths(UIno_EditorSubsystem::StaticClass(), Paths);

	// UAssetManager::Get().
	// 	ScanPathsForPrimaryAssets(PrimaryAssetType_InoSubsystem, Paths.Array(),
	// 	                          UIno_GameInstanceSubsystem::StaticClass(), true, false, true);
	// UAssetManager::Get().
	// 	ScanPathsForPrimaryAssets(PrimaryAssetType_InoSubsystem, Paths.Array(),
	// 	                          UIno_LocalPlayerSubsystem::StaticClass(), true, false, true);
	// UAssetManager::Get().
	// 	ScanPathsForPrimaryAssets(PrimaryAssetType_InoSubsystem, Paths.Array(),
	// 	                          UIno_WorldSubsystem::StaticClass(), true, false, true);

	UE_LOG(LogInoSubsystem, Log, TEXT("End of load All of InoSubsystem Asset"))
}
