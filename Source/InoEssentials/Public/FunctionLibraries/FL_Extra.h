/* Copyright (c) 2021-2024 by Inoland */

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "FL_Extra.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (DisplayName = "Ino FL Extra", Keywords = "extra"), Category = "Ino|FL|Extra")
class UFL_Extra : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFL_Extra(const FObjectInitializer& ObjectInitializer);
	
public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Map Name", Keywords = "get map name",HidePin = "WorldContextObject", WorldContext="WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "Ino|FL|Extra")
	static FString GetMapName(UObject* WorldContextObject, bool RemovePrefix);
	
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Ino Base64 Encode"), Category = "Ino|FL|Extra")
		static void Base64Encode(const FString& InString, FString &EncodedString);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Ino Base64 Decode"), Category = "Ino|FL|Extra")
		static bool Base64Decode(const FString& EncodedString, FString &DecodedString);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Ino Base64 Encode Data"), Category = "Ino|FL|Extra")
	static bool Base64EncodeData(const TArray<uint8>& Data, FString& EncodedData);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Ino Base64 Decode Data"), Category = "Ino|FL|Extra")
	static bool Base64DecodeData(const FString& EncodedData, TArray<uint8>& DecodedData);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Ino Hash MD5"), Category = "Ino|FL|Extra")
	static FString HashMD5(const FString& StringToHash);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Ino Hash SHA1"), Category = "Ino|FL|Extra")
	static FString HashSHA1(const FString& StringToHash);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Convert TextureRenderTargetCube to TextureCube"), Category = "Ino|FL|Extra")
		static UTextureCube* TRTC_TC( UTextureRenderTargetCube* InputRenderTargetCube, UObject* InOuter, FString Name = "Test");

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reflection Capture Component Set Cube"), Category = "Ino|FL|Extra")
		static bool RCC_SetCube( UReflectionCaptureComponent* ReflectionCapture, UTextureCube* InputCube);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Load String From File"), Category = "Ino|FL|Extra")
		static bool LoadStringFromFile(const FString& FilePath, FString& LoadedString);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Save String To File"), Category = "Ino|FL|Extra")
		static bool SaveStringToFile(const FString& InputString, const FString& FilePath, const bool Append);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get File Size"), Category = "Ino|FL|Extra")
		static bool GetFileSize(const FString& FilePath, int64& FileSize);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get local IP address"), Category = "Ino|FL|Extra")
    	static FString GetLocalIP();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Hash String With MD5"), Category = "Ino|FL|Extra")
		static FString HashStringWithMD5(const FString& Input);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Encrypt AES"), Category = "Ino|FL|Extra")
		static FString EncryptAES(const FString& PlainText, const FString& Key);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Decrypt AES"), Category = "Ino|FL|Extra")
		static FString DecryptAES(const FString& EncryptedText, const FString& Key);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Encrypt AES"), Category = "Ino|FL|Extra")
	static FString EncryptSH1(const FString& PlainText, const FString& Key);

	

};
