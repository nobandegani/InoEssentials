/* Copyright (c) 2021-2023 by Inoland */

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "FL_EditorBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (DisplayName = "Ino Editor FL Base", Keywords = "base"), Category = "Ino|Editor|Base")
class UFL_EditorBase : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFL_EditorBase(const FObjectInitializer& ObjectInitializer);
	
public:

	UFUNCTION(BlueprintCallable, CallInEditor, meta = (DisplayName = "Get Project Version"), Category = "Ino|Editor|Base")
		static FString GetProjectVersion();

	UFUNCTION(BlueprintCallable, CallInEditor, meta = (DisplayName = "Set Project Version"), Category = "Ino|Editor|Base")
		static void SetProjectVersion(FString Version);
	
	UFUNCTION(BlueprintCallable, CallInEditor, meta = (DisplayName = "Get Android Version"), Category = "Ino|Editor|Base")
		static void GetAndroidVersion(int32 &StoreVersion, FString &VersionDisplayName);

	UFUNCTION(BlueprintCallable, CallInEditor, meta = (DisplayName = "Set Android Version"), Category = "Ino|Editor|Base")
		static void SetAndroidVersion(int32 StoreVersion, FString VersionDisplayName);

	UFUNCTION(BlueprintCallable, CallInEditor,DisplayName="Open Directory Dialog", Category = "Ino|Editor|Base")
		static bool OpenDirectoryDialog(FString Title, FString DefaultPath, FString &SelectedPath);

	UFUNCTION(BlueprintCallable, CallInEditor,DisplayName="Get Files From Directory", Category = "Ino|Editor|Base")
		static bool GetFilesFromDirectory(const FString& Directory, TArray<FString>& OutFileList);

	UFUNCTION(BlueprintCallable, CallInEditor,DisplayName="Get File Size", Category = "Ino|Editor|Base")
		static bool GetFileSize(const FString& FileDirectory, int64& FileSize);

	UFUNCTION(BlueprintCallable, CallInEditor,DisplayName="Save String To File", Category = "Ino|Editor|Base")
		static bool SaveStringToFile(const FString StringToSave, const FString FileName, const FString FileDirectory);

	UFUNCTION(BlueprintCallable, CallInEditor,DisplayName="Load String From File", Category = "Ino|Editor|Base")
		static bool LoadStringFromFile(const FString FileName, const FString FileDirectory, FString& LoadedString);

	UFUNCTION(BlueprintCallable, CallInEditor,DisplayName="Set String In Editor Config", Category = "Ino|Editor|Base")
		static bool SetStringInEditorConfig(const FString SectionName, const FString KeyName, const FString Value, bool Flush);

	UFUNCTION(BlueprintCallable, CallInEditor,DisplayName="Get String From Editor Config", Category = "Ino|Editor|Base")
		static bool GetStringFromEditorConfig(const FString SectionName, const FString KeyName, FString& Value);
};
