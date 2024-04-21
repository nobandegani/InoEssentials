/* Copyright (c) 2021-2023 by Inoland */

#include "FunctionLibraries/FL_EditorBase.h"

#include "AndroidRuntimeSettings.h"
#include "DesktopPlatformModule.h"
#include "GeneralProjectSettings.h"
#include "Misc/FileHelper.h"

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

bool UFL_EditorBase::GetFileSize(const FString& FileDirectory, int64& FileSize)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	
	if (PlatformFile.FileExists(*FileDirectory))
	{
		FileSize = PlatformFile.FileSize(*FileDirectory);
		return true;
	}
	return false;
}

bool UFL_EditorBase::SaveStringToFile(const FString StringToSave, const FString FileName, const FString FileDirectory)
{
	FString FilePath = FileDirectory + FileName;

	return FFileHelper::SaveStringToFile(StringToSave, *FilePath);
}

bool UFL_EditorBase::LoadStringFromFile(const FString FileName, const FString FileDirectory, FString& LoadedString)
{
	FString FilePath = FileDirectory + FileName;
	return FFileHelper::LoadFileToString(LoadedString, *FilePath);
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
