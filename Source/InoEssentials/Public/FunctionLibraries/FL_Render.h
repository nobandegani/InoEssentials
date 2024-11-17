/* Copyright (c) 2021-2024 by Inoland */

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

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
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reflection Capture Component Set Cube"), Category = "Ino|FL|Extra")
		static bool RCC_SetCube( UReflectionCaptureComponent* ReflectionCapture, UTextureCube* InputCube);
	
	UFUNCTION(BlueprintCallable, Category = "Ino|FL|Extra")
		static UTexture2D* CreateTexture2dFromRenderTarget2d(UObject* WorldContextObject, UTextureRenderTarget2D* RenderTarget);

	UFUNCTION(BlueprintCallable, Category = "Ino|FL|Extra")
		static UTexture2D* CreateR16FTextureFromRenderTargetAlpha(UObject* WorldContextObject, UTextureRenderTarget2D* RenderTarget);

	UFUNCTION(BlueprintCallable, Category = "Render Target")
		static void CopyRenderTargetToRenderTargetByRenderThread(UTextureRenderTarget2D* SourceRenderTarget, UTextureRenderTarget2D* DestinationRenderTarget);

	UFUNCTION(BlueprintCallable, Category = "Render Target")
		static void CopyRenderTargetToRenderTargetByCPU(UTextureRenderTarget2D* SourceRenderTarget, UTextureRenderTarget2D* DestinationRenderTarget);
	
	UFUNCTION(BlueprintCallable, Category = "Render Target")
		static FVector GetWorldPositionFromRenderTarget(UTextureRenderTarget2D* RenderTarget, int32 X, int32 Y);
};
