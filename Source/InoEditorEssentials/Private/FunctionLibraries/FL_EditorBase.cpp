/* Copyright (c) 2021-2023 by Inoland */

#include "FunctionLibraries/FL_EditorBase.h"

#include "AndroidRuntimeSettings.h"
#include "DesktopPlatformModule.h"
#include "GeneralProjectSettings.h"
#include "Misc/FileHelper.h"

#include "Engine/TextureRenderTargetCube.h"
#include "Engine/TextureRenderTarget2D.h"

UFL_EditorBase::UFL_EditorBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

FString UFL_EditorBase::GetProjectVersion()
{
	UGeneralProjectSettings* ProjectSettings = GetMutableDefault<UGeneralProjectSettings>();
	if (ProjectSettings == nullptr)
	{
		return "Unkown";
	}
	return ProjectSettings->ProjectVersion;
}

void UFL_EditorBase::SetProjectVersion(FString Version)
{
	UGeneralProjectSettings* ProjectSettings = GetMutableDefault<UGeneralProjectSettings>();
	if (ProjectSettings == nullptr)
	{
		return;
	}
	
	ProjectSettings->ProjectVersion = Version;
	ProjectSettings->SaveConfig();
	ProjectSettings->TryUpdateDefaultConfigFile();
}

void UFL_EditorBase::GetAndroidVersion(int32& StoreVersion, FString& VersionDisplayName)
{
	UAndroidRuntimeSettings* AndroidRuntimeSettings = GetMutableDefault<UAndroidRuntimeSettings>();
	if (AndroidRuntimeSettings == nullptr)
	{
		return ;
	}
	StoreVersion = AndroidRuntimeSettings->StoreVersion;
	VersionDisplayName = AndroidRuntimeSettings->VersionDisplayName;
}


void UFL_EditorBase::SetAndroidVersion(int32 StoreVersion, FString VersionDisplayName)
{
	UAndroidRuntimeSettings* AndroidRuntimeSettings = GetMutableDefault<UAndroidRuntimeSettings>();
	if (AndroidRuntimeSettings == nullptr)
	{
		return ;
	}

	AndroidRuntimeSettings->StoreVersion = StoreVersion;
	AndroidRuntimeSettings->VersionDisplayName = VersionDisplayName;
	AndroidRuntimeSettings->SaveConfig();
	AndroidRuntimeSettings->TryUpdateDefaultConfigFile();
}

bool UFL_EditorBase::OpenDirectoryDialog(FString Title, FString DefaultPath, FString &SelectedPath)
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		return DesktopPlatform->OpenDirectoryDialog(nullptr, Title, DefaultPath, SelectedPath);
		
	}
	return false;
}

bool UFL_EditorBase::GetFilesFromDirectory(const FString& Directory, TArray<FString>& OutFileList)
{
	// Get the file manager
	IFileManager& FileManager = IFileManager::Get();
	
	FileManager.FindFiles(OutFileList, *Directory, nullptr);
	
	return true;
}

bool UFL_EditorBase::SetStringInEditorConfig(const FString SectionName, const FString KeyName, const FString Value, bool Flush)
{
	GConfig->SetString(*SectionName, *KeyName, *Value, GEditorIni);
	if (Flush)
	{
		GConfig->Flush(false, GEditorIni);
	}
	return true;
}

bool UFL_EditorBase::GetStringFromEditorConfig(const FString SectionName, const FString KeyName, FString& Value)
{
	return GConfig->GetString(*SectionName, *KeyName, Value, GEditorIni);
}

UTexture2D* UFL_EditorBase::TRT2D_T2D(UTextureRenderTarget2D* InputRenderTarget2D, UObject* InOuter, FString Name)
{
	return InputRenderTarget2D->ConstructTexture2D(InOuter, Name, EObjectFlags::RF_Public);
}

UTextureCube* UFL_EditorBase::TRTC_TC(UTextureRenderTargetCube* InputRenderTargetCube, UObject* InOuter, FString Name)
{
	return InputRenderTargetCube->ConstructTextureCube(InOuter, Name, EObjectFlags::RF_Public);
}
