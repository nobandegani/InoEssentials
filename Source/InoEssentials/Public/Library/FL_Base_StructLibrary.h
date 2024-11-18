/* Copyright (c) 2021-2024 by Inoland */

#pragma once

#include "FL_Base_StructLibrary.generated.h"

UENUM(BlueprintType)
enum class EInoLogType : uint8
{
	Display = 0,
	Success = 1,
	Warning = 2,
	Error = 3,
	Fatal = 4,
};

USTRUCT(BlueprintType)
struct FInoLogConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FInoLogConfig")
	bool bPrintToScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FInoLogConfig")
	bool bPrintToLog;
	
	FInoLogConfig()
		: bPrintToScreen(true), bPrintToLog(true)
	{
	}
};

USTRUCT(BlueprintType)
struct FInoLogPreset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InoLogPreset")
	bool bActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InoLogPreset")
	float Duration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InoLogPreset")
    	bool bDrawDebug;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InoLogPreset")
	FInoLogConfig Display;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InoLogPreset")
	FInoLogConfig Success;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InoLogPreset")
	FInoLogConfig Warning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InoLogPreset")
	FInoLogConfig Error;
	
	FInoLogPreset()
		: bActive(true), Duration(20.f), bDrawDebug(true), Display(), Success(), Warning(), Error()
	{
	}
};




UENUM(BlueprintType)
enum class EViewMode : uint8
{
	None,
	GameAndUI,
	UIOnly,
	GameOnly,
};

UENUM(BlueprintType)
enum class EInoCompressor : uint8
{
	NotSet = 0,
	Selkie = 1,
	Mermaid = 2,
	Kraken  = 3,
	Leviathan = 4
};

UENUM(BlueprintType)
enum class EInoCompressionLevel : uint8
{
	HyperFast4 = 0,
	HyperFast3 = 1,
	HyperFast2 = 2,
	HyperFast1 = 3,
	None = 4,
	SuperFast = 5,
	VeryFast = 6,
	Fast = 7,
	Normal = 8,
	Optimal1 = 9,
	Optimal2 = 10,
	Optimal3 = 11,
	Optimal4 = 12,
	Optimal5 = 13,// if another added update CompressionLevelNameMap
};

UENUM(BlueprintType)
enum class EInoDataType : uint8
{
	Bit8 = 0,
	Bit16 = 1,
	Bit32 = 2,
	Bit64 = 4
};

UENUM(BlueprintType)
enum class EInoChannelType : uint8
{
	R = 0,
	G = 1,
	B = 2,

	RGB = 3,
	RGBA = 4,
	
	RG = 5,
	RB = 6,
	GB = 7,
	
	A = 8,
	RA = 9,
	GA = 10,
	BA = 11,
};