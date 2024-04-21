/* Copyright (c) 2021-2024 by Inoland */

#pragma once

#include "FL_Base_StructLibrary.generated.h"

USTRUCT(BlueprintType)
struct FInoTest
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, DisplayName = "Audible Distance", Category = "3D Properties")
	int32 audibleDistance = 2700;
	
	FInoTest ()
	{}
};

UENUM(BlueprintType)
enum class ELogType : uint8
{
	Unkown = 0,
	Info = 1,
	Warning = 2,
	Success = 3,
	Error = 4,
	Critical = 5,
};

UENUM(BlueprintType)
enum class ELogCategory : uint8
{
	Base = 0,
	InoEsseintials = 1,
	InoOSS = 2,
};

UENUM(BlueprintType)
enum class EViewMode : uint8
{
	None,
	GameAndUI,
	UIOnly,
	GameOnly,
};