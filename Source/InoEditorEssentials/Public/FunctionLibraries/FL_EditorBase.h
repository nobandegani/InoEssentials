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
	
	UFUNCTION(BlueprintCallable, CallInEditor,DisplayName="Set String In Editor Config", Category = "Ino|Editor|Base")
		static bool SetStringInEditorConfig(const FString SectionName, const FString KeyName, const FString Value, bool Flush);

	UFUNCTION(BlueprintCallable, CallInEditor,DisplayName="Get String From Editor Config", Category = "Ino|Editor|Base")
		static bool GetStringFromEditorConfig(const FString SectionName, const FString KeyName, FString& Value);

	UFUNCTION(BlueprintCallable, CallInEditor, meta = (DisplayName = "Convert TextureRenderTarget2D to Texture2D"), Category = "Ino|Editor|Base")
	static UTexture2D* TRT2D_T2D( UTextureRenderTarget2D* InputRenderTarget2D, UObject* InOuter, FString Name = "Test");
	
	UFUNCTION(BlueprintCallable, CallInEditor, meta = (DisplayName = "Convert TextureRenderTargetCube to TextureCube"), Category = "Ino|Editor|Base")
	static UTextureCube* TRTC_TC( UTextureRenderTargetCube* InputRenderTargetCube, UObject* InOuter, FString Name = "Test");
};
