/* Copyright (c) 2021-2024 by Inoland */

#pragma once

#include "Subsystems/SS_GI.h"

#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"

#include "SS_InoBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(InoBase, Log, All);

UCLASS(Abstract, Within = GameInstance, Blueprintable, BlueprintType, DisplayName="InoCore Subsystem", Category="Ino|SS|Base")
class INOESSENTIALS_API UInoBase : public UInoGISS
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent, Category="Ino|SS|Base")
	void InoLogCalled(const FString& Input, const FGameplayTag& Type, const FGameplayTag& Category, const FString& ObjectName, bool InoLog, bool ServerLog);
public:
	
};

