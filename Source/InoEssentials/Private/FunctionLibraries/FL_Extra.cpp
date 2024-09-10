/* Copyright (c) 2021-2023 by Inoland */

#include "FunctionLibraries/FL_Extra.h"

#include <string>


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

void UFL_Extra::Base64Encode(const FString& InString, FString &EncodedString)
{
	EncodedString = FBase64::Encode(InString);
}

bool UFL_Extra::Base64Decode(const FString& EncodedString, FString& DecodedString)
{
	return FBase64::Decode(EncodedString, DecodedString);
}

bool UFL_Extra::Base64EncodeData(const TArray<uint8>& Data, FString& EncodedData)
{
	if (Data.Num() > 0)
	{
		EncodedData = FBase64::Encode(Data);
		return true;
	}

	EncodedData = FString();
	return false;
}

bool UFL_Extra::Base64DecodeData(const FString& EncodedData, TArray<uint8>& DecodedData)
{
	return FBase64::Decode(EncodedData, DecodedData);
}

FString UFL_Extra::HashMD5(const FString& StringToHash)
{
	return FMD5::HashAnsiString(*StringToHash);
}

FString UFL_Extra::HashSHA1(const FString& StringToHash)
{
	FSHA1 Sha1Gen;

	Sha1Gen.Update((unsigned char*)TCHAR_TO_ANSI(*StringToHash), FCString::Strlen(*StringToHash));
	Sha1Gen.Final();

	FString Sha1String;
	for (int32 i = 0; i < 20; i++)
	{
		Sha1String += FString::Printf(TEXT("%02x"), Sha1Gen.m_digest[i]);
	}

	return Sha1String;
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

FString UFL_Extra::EncryptAES(const FString& PlainText, const FString& Key)
{
	// Check if the key is exactly 32 bytes
	if (Key.Len() != 32)
	{
		return TEXT("Key should be 32 bytes");
	}

	// Convert FString to TArray<uint8> for Content
	TArray<uint8> ContentBytes;
	ContentBytes.Append(reinterpret_cast<const uint8*>(*PlainText), PlainText.Len());

	// Add padding to ensure the size is a multiple of 16 bytes
	int32 PaddingSize = 16 - (ContentBytes.Num() % 16);
	ContentBytes.AddZeroed(PaddingSize);

	// Convert FString to TArray<uint8> for Key
	TArray<uint8> KeyBytes;
	KeyBytes.Append(reinterpret_cast<const uint8*>(*Key), Key.Len());

	// Prepare an array to hold the encrypted data
	TArray<uint8> EncryptedBytes;
	EncryptedBytes.SetNum(ContentBytes.Num());

	// Encrypt the data
	FAES::EncryptData(EncryptedBytes.GetData(), ContentBytes.Num(), KeyBytes.GetData(), KeyBytes.Num());

	// Convert TArray<uint8> back to FString (Base64 encoding is common for binary data)
	FString EncryptedString = FBase64::Encode(EncryptedBytes);

	return EncryptedString;
}

FString UFL_Extra::DecryptAES(const FString& EncryptedText, const FString& Key)
{
	// Check if the key is exactly 32 bytes
	if (Key.Len() != 32)
	{
		return TEXT("Key should be 32 bytes");
	}

	// Convert the FString (Base64 encoded) to TArray<uint8>
	TArray<uint8> EncryptedBytes;
	FBase64::Decode(EncryptedText, EncryptedBytes);

	// Convert FString to TArray<uint8> for Key
	TArray<uint8> KeyBytes;
	KeyBytes.Append(reinterpret_cast<const uint8*>(*Key), Key.Len());

	// Decrypt the data
	FAES::DecryptData(EncryptedBytes.GetData(), EncryptedBytes.Num(), KeyBytes.GetData(), KeyBytes.Num());

	// Remove padding (remove trailing zeros)
	int32 ActualDataSize = EncryptedBytes.Num();
	while (ActualDataSize > 0 && EncryptedBytes[ActualDataSize - 1] == 0)
	{
		--ActualDataSize;
	}

	// Convert decrypted TArray<uint8> back to FString
	FString DecryptedString = FString(reinterpret_cast<const char*>(EncryptedBytes.GetData()), ActualDataSize);

	return DecryptedString;
}