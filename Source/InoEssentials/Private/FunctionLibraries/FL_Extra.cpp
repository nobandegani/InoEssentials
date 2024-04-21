/* Copyright (c) 2021-2023 by Inoland */

#include "FunctionLibraries/FL_Extra.h"

#include "Misc/Base64.h"

#include "Kismet/GameplayStatics.h"

#include "Components/ReflectionCaptureComponent.h"
#include "Engine/TextureRenderTargetCube.h"

#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"



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


void UFL_Extra::SetWorldContextForAllInoGISS(const UObject* WorldContextObject)
{
	if (WorldContextObject == nullptr || !IsValid(WorldContextObject))
	{
		return;
	}
	if ( WorldContextObject->GetWorld() == nullptr)
	{
		return;
	}
	
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	if (GameInstance == nullptr)
	{
		return;
	}
	
	TArray<UInoGISS*> GameInstances = GameInstance->GetSubsystemArray<UInoGISS>();
	for (auto InoGISS : GameInstances)
	{
		InoGISS->SetWorldContext(WorldContextObject);
	}
}

void UFL_Extra::SetWorldContextForInoGISS(const UObject* WorldContextObject, UInoGISS* InputInoGISS)
{
	if (WorldContextObject == nullptr || !IsValid(WorldContextObject))
	{
		return;
	}
	if ( WorldContextObject->GetWorld() == nullptr)
	{
		return;
	}
	
	if (InputInoGISS == nullptr)
	{
		return;
	}
	
	InputInoGISS->SetWorldContext(WorldContextObject);
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

bool UFL_Extra::SaveStringToFile(const FString& InputString, const FString& FilePath, const bool Append)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString Directory = FPaths::GetPath(FilePath);
	if (!PlatformFile.DirectoryExists(*Directory))
	{
		PlatformFile.CreateDirectory(*Directory);
	}

	// Append the string to the file
	return FFileHelper::SaveStringToFile(InputString, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), Append ? FILEWRITE_Append : FILEWRITE_None);
}
