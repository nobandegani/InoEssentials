/* Copyright (c) 2021-2024 by Inoland */

#include "InoSubsystemEditor.h"

#include "AssetToolsModule.h"

#include "InoSubsystemTypeActions.h"

#include "Ino_LocalPlayerSubsystem.h"
#include "Ino_GameInstanceSubsystemTickable.h"
#include "Ino_LocalPlayerSubsystemTickable.h"
#include "Ino_WorldSubsystemTickable.h"
#include "Ino_WorldSubsystem.h"

#include "IAssetTools.h"

#define LOCTEXT_NAMESPACE "FInoSubsystemEditorModule"

void FInoSubsystemEditorModule::StartupModule()
{
	// GetAssetTool
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	// Spawn AssetTypeActions
	ActionTypes.Append
	(
		{
			MakeShareable(new FInoSubsystemTypeActionsTemplate<UIno_GameInstanceSubsystem>()),
			MakeShareable(new FInoSubsystemTypeActionsTemplate<UIno_WorldSubsystem>()),
			MakeShareable(new FInoSubsystemTypeActionsTemplate<UIno_LocalPlayerSubsystem>()),
			MakeShareable(new FInoSubsystemTypeActionsTemplate<UIno_GameInstanceSubsystemTickable>()),
			MakeShareable(new FInoSubsystemTypeActionsTemplate<UIno_WorldSubsystemTickable>()),
			MakeShareable(new FInoSubsystemTypeActionsTemplate<UIno_LocalPlayerSubsystemTickable>())
		}
	);

	// Register AssetTypeAction
	for (auto Action : ActionTypes)
	{
		AssetTools.RegisterAssetTypeActions(Action.ToSharedRef());
	}
}

void FInoSubsystemEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::Get().GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (auto Action : ActionTypes)
		{
			AssetTools.UnregisterAssetTypeActions(Action.ToSharedRef());
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FInoSubsystemEditorModule, InoSubsystemEditor)
