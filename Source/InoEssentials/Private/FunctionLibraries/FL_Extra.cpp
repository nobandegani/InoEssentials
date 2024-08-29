/* Copyright (c) 2021-2023 by Inoland */

#include "FunctionLibraries/FL_Extra.h"


#include "Misc/Base64.h"

#include "Kismet/GameplayStatics.h"

#include "Components/ReflectionCaptureComponent.h"
#include "Engine/TextureRenderTargetCube.h"

#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"

#include "HAL/PlatformMisc.h"
#include "SocketSubsystem.h"

#include "Misc/SecureHash.h"

UFL_Extra::UFL_Extra(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

FString UFL_Extra::GetMapName(UObject* WorldContextObject, bool RemovePrefix)
{
	if (WorldContextObject == nullptr)
	{
		return "invalid world context object";
	}

	if (WorldContextObject->GetWorld() == nullptr)
	{
		return "invalid World";
	}
	
	FString MapName = WorldContextObject->GetWorld()->GetMapName();

	if (RemovePrefix)
	{
		MapName.RemoveFromStart(WorldContextObject->GetWorld()->StreamingLevelsPrefix);
	}
	
	return MapName;
}

void UFL_Extra::Base64Encode(FString InString, FString &EncodedString)
{
	EncodedString = FBase64::Encode(InString);
}

void UFL_Extra::Base64Decode(FString EncodedString, FString& DecodedString, bool& Success)
{
	Success = FBase64::Decode(EncodedString, DecodedString);
}

UTextureCube* UFL_Extra::TRTC_TC(UTextureRenderTargetCube* InputRenderTargetCube, UObject* InOuter, FString Name)
{
	return InputRenderTargetCube->ConstructTextureCube(InOuter, Name, EObjectFlags::RF_Public);
}

bool UFL_Extra::RCC_SetCube(UReflectionCaptureComponent* ReflectionCapture, UTextureCube* InputCube)
{
	if (ReflectionCapture == nullptr || InputCube == nullptr)
	{
		return false;
	}
	ReflectionCapture->Cubemap = InputCube;
	return true;
}

bool UFL_Extra::LoadStringFromFile(const FString& FilePath, FString& LoadedString)
{
	return FFileHelper::LoadFileToString(LoadedString, *FilePath);
}

bool UFL_Extra::SaveStringToFile(const FString& InString, const FString& InFilePath, const bool Append)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString Directory = FPaths::GetPath(InFilePath);
	if (!PlatformFile.DirectoryExists(*Directory))
	{
		PlatformFile.CreateDirectory(*Directory);
	}

	// Append the string to the file
	return FFileHelper::SaveStringToFile(InString, *InFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), Append ? FILEWRITE_Append : FILEWRITE_None);
}

bool UFL_Extra::GetFileSize(const FString& FilePath, int64& FileSize)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	
	if (!PlatformFile.FileExists(*FilePath))
	{
		return false;
	}

	FileSize = PlatformFile.FileSize(*FilePath);
	return true;
}

FString UFL_Extra::GetLocalIP()
{
	bool bCanBindAll;
	TSharedPtr<FInternetAddr> LocalIP = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBindAll);
	if (LocalIP.IsValid())
	{
		return LocalIP->ToString(false);  // 'false' to get the address without the port number
	}

	return "Unable to retrieve IP Address";
}

FString UFL_Extra::HashStringWithMD5(const FString& Input)
{
	// Convert the FString to an ANSI string for hashing
	FTCHARToUTF8 Convert(*Input);
	const ANSICHAR* UTF8String = reinterpret_cast<const ANSICHAR*>(Convert.Get());

	// Compute the MD5 hash
	FMD5 MD5;
	MD5.Update(reinterpret_cast<const uint8*>(UTF8String), FCStringAnsi::Strlen(UTF8String));

	uint8 Hash[16];
	MD5.Final(Hash);

	// Convert the hash to a readable hex string
	FString MD5Hash;
	for (int32 i = 0; i < 16; i++)
	{
		MD5Hash += FString::Printf(TEXT("%02x"), Hash[i]);
	}

	return MD5Hash;
}
