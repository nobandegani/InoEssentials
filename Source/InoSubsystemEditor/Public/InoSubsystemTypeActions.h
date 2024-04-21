/* Copyright (c) 2021-2024 by Inoland */

#pragma once

#include "CoreMinimal.h"
#include "Ino_GameInstanceSubsystem.h"
#include "GraphEditorSettings.h"
#include "AssetTypeActions/AssetTypeActions_Blueprint.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

class INOSUBSYSTEMEDITOR_API FInoSubsystemTypeActionsBase : public FAssetTypeActions_Base
{
public:
	virtual FColor GetTypeColor() const override { return FColor(63, 126, 255); }
	virtual uint32 GetCategories() override
	{
		static EAssetTypeCategories::Type const AssetCategory = EAssetTypeCategories::Blueprint;
		return AssetCategory;
	}
	virtual const TArray<FText>& GetSubMenus() const override
	{
		static const TArray<FText> SubMenus
		{
			LOCTEXT("InoSubsystemClassSubMenu", "Subsystems")
		};
		return SubMenus;
	}
};

template <typename SubsystemClass=USubsystem>
class INOSUBSYSTEMEDITOR_API FInoSubsystemTypeActionsTemplate : public FInoSubsystemTypeActionsBase
{
public:
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override { return SubsystemClass::StaticClass(); }
};

template <typename SubsystemClass>
FText INOSUBSYSTEMEDITOR_API FInoSubsystemTypeActionsTemplate<SubsystemClass>::GetName() const
{
	FString ClassName = SubsystemClass::StaticClass()->GetName();
	int Index;
	ClassName.FindChar('_', Index);
	return FText::FromString(ClassName.RightChop(Index + 1));
}
#undef LOCTEXT_NAMESPACE
