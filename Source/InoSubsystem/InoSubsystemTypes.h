/* Copyright (c) 2021-2024 by Inoland */

#pragma once

#include "Logging/LogMacros.h"

static FPrimaryAssetType const PrimaryAssetType_InoSubsystem = TEXT("InoSubsystem");

DECLARE_LOG_CATEGORY_EXTERN(LogInoSubsystem,Log,All);
DEFINE_LOG_CATEGORY(LogInoSubsystem);
// #define Ino_Subsystem_PrimaryAssetType FPrimaryAssetType("Ino_Subsystem")
// #define Ino_Subsystem_PrimaryAssetId(InAssetName) FPrimaryAssetId(Ino_Subsystem_PrimaryAssetType,InAssetName)
