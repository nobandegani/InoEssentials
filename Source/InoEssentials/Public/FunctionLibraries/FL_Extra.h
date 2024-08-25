/* Copyright (c) 2021-2024 by Inoland */

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "FL_Extra.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (DisplayName = "Ino Extra FL", Keywords = "extra"), Category = "Ino Extra FL")
class UFL_Extra : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFL_Extra(const FObjectInitializer& ObjectInitializer);
	
public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Map Name", Keywords = "get map name",HidePin = "WorldContextObject", WorldContext="WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "Ino Extra FL")
	static FString GetMapName(UObject* WorldContextObject, bool RemovePrefix);
	
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Ino Base64 Encode"), Category = "Ino Extra FL")
		static void Base64Encode(FString InString, FString &EncodedString);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Ino Base64 Decode"), Category = "Ino Extra FL")
		static void Base64Decode(FString EncodedString, FString &DecodedString, bool &Success);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Convert TextureRenderTargetCube to TextureCube"), Category = "Ino Extra FL")
		static UTextureCube* TRTC_TC( UTextureRenderTargetCube* InputRenderTargetCube, UObject* InOuter, FString Name = "Test");

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reflection Capture Component Set Cube"), Category = "Ino Extra FL")
		static bool RCC_SetCube( UReflectionCaptureComponent* ReflectionCapture, UTextureCube* InputCube);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Load String From File"), Category = "Ino Extra FL")
		static bool LoadStringFromFile(const FString& FilePath, FString& LoadedString);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Save String To File"), Category = "Ino Extra FL")
		static bool SaveStringToFile(const FString& InputString, const FString& FilePath, const bool Append);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get File Size"), Category = "Ino Extra FL")
		static bool GetFileSize(const FString& FilePath, int64& FileSize);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get local IP address"), Category = "Ino Extra FL")
    	static FString GetLocalIP();
};
