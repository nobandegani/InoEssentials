/* Copyright (c) 2021-2024 by Inoland */

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Library/FL_Base_StructLibrary.h"

#include "FL_Render.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (DisplayName = "Ino FL Render", Keywords = "render"), Category = "Ino|FL|Render")
class UFL_Render : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFL_Render(const FObjectInitializer& ObjectInitializer);
	
public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reflection Capture Component Set Cube"), Category = "Ino|FL|Render")
		static bool RCC_SetCube( UReflectionCaptureComponent* ReflectionCapture, UTextureCube* InputCube);
	
	UFUNCTION(BlueprintCallable, Category = "Ino|FL|Render")
		static UTexture2D* CreateTexture2dFromRenderTarget2d(UObject* WorldContextObject, UTextureRenderTarget2D* RenderTarget);

	UFUNCTION(BlueprintCallable, Category = "Ino|FL|Render")
		static UTexture2D* CreateR16FTextureFromRenderTargetAlpha(UObject* WorldContextObject, UTextureRenderTarget2D* RenderTarget);

	UFUNCTION(BlueprintCallable, Category = "Ino|FL|Render")
		static void CopyRenderTargetToRenderTargetByRenderThread(UTextureRenderTarget2D* SourceRenderTarget, UTextureRenderTarget2D* DestinationRenderTarget);

	UFUNCTION(BlueprintCallable, Category = "Ino|FL|Render")
		static void CopyRenderTargetToRenderTargetByCPU(UTextureRenderTarget2D* SourceRenderTarget, UTextureRenderTarget2D* DestinationRenderTarget);
	
	UFUNCTION(BlueprintCallable, Category = "Ino|FL|Render")
		static FVector GetWorldPositionFromRenderTarget(UTextureRenderTarget2D* RenderTarget, int32 X, int32 Y);

	UFUNCTION(BlueprintCallable, Category = "Ino|FL|Render", meta = (Keywords = "ReadRenderTarget", WorldContext = "WorldContextObject"))
		static FLinearColor ReadRawPixelFromRenderTarget(UObject* WorldContextObject, UTextureRenderTarget2D* TextureRenderTarget, int32 X, int32 Y, bool bNormalize = true);

	UFUNCTION(BlueprintCallable, Category = "Ino|FL|Render", meta = (Keywords = "ReadRenderTarget", WorldContext = "WorldContextObject"))
		static bool ReadRawPixelsFromRenderTarget(UObject* WorldContextObject, UTextureRenderTarget2D* TextureRenderTarget, TArray<FLinearColor>& OutLinearSamples, bool bNormalize = true);

	UFUNCTION(BlueprintCallable, Category = "Ino|FL|Render", meta = (Keywords = "ReadRenderTarget", WorldContext = "WorldContextObject"))
		static bool SavePixelsToFile(
			UObject* WorldContextObject,
			const TArray<FLinearColor>& InLinearSamples,
			const FString& InFilePath,
			const bool bOverride,
			const bool bCompress,
			EInoCompressor InCompressor,
			EInoCompressionLevel InCompressionLevel,
			FString& OutFilePath);
	

	UFUNCTION(BlueprintCallable, Category = "Ino|FL|Render", meta = (Keywords = "ReadRenderTarget", WorldContext = "WorldContextObject"))
		static bool LoadPixelsFromFile(
			UObject* WorldContextObject,
			const FString& FilePath,
			const bool bDeCompress,
			TArray<FLinearColor>& InLinearSamples);

	UFUNCTION(BlueprintCallable, Category = "Ino|FL|Render")
		static bool DeserializeLinearColorArray(const TArray<uint8>& Data, TArray<FLinearColor>& OutLinearSamples);
private:
	
};
