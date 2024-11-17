/* Copyright (c) 2021-2023 by Inoland */

#include "FunctionLibraries/FL_Render.h"

#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"

#include "Components/ReflectionCaptureComponent.h"

#include "Engine/TextureRenderTargetCube.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"

#include "RenderUtils.h"
#include "RHI.h"
#include "RHICommandList.h"


UFL_Render::UFL_Render(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool UFL_Render::RCC_SetCube(UReflectionCaptureComponent* ReflectionCapture, UTextureCube* InputCube)
{
	if (ReflectionCapture == nullptr || InputCube == nullptr)
	{
		return false;
	}
	ReflectionCapture->Cubemap = InputCube;
	return true;
}

UTexture2D* UFL_Render::CreateTexture2dFromRenderTarget2d(UObject* WorldContextObject,
	UTextureRenderTarget2D* RenderTarget)
{
	if (!RenderTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Render Target"));
		return nullptr;
	}

	// Read pixel data from the render target
	TArray<FColor> SurfaceData;
	FRenderTarget* RenderTargetResource = RenderTarget->GameThread_GetRenderTargetResource();
	FReadSurfaceDataFlags ReadPixelFlags(RCM_UNorm);
	ReadPixelFlags.SetLinearToGamma(false);
	RenderTargetResource->ReadPixels(SurfaceData, ReadPixelFlags);

	// Create a new Texture2D
	UTexture2D* NewTexture = UTexture2D::CreateTransient(RenderTarget->GetSurfaceWidth(), RenderTarget->GetSurfaceHeight(), PF_B8G8R8A8);
	if (!NewTexture)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create transient texture"));
		return nullptr;
	}

	void* TextureData = NewTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextureData, SurfaceData.GetData(), SurfaceData.Num() * sizeof(FColor));
	NewTexture->GetPlatformData()->Mips[0].BulkData.Unlock();

	NewTexture->UpdateResource();

	UE_LOG(LogTemp, Log, TEXT("Successfully created texture from render target"));

	return NewTexture;
}

UTexture2D* UFL_Render::CreateR16FTextureFromRenderTargetAlpha(UObject* WorldContextObject,
	UTextureRenderTarget2D* RenderTarget)
{
	if (!RenderTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Render Target"));
		return nullptr;
	}

	// Prepare an array to hold the pixel data
	TArray<FFloat16Color> FloatSurfaceData;

	// Read float pixel data from the render target (this reads R, G, B, A as float values)
	FRenderTarget* RenderTargetResource = RenderTarget->GameThread_GetRenderTargetResource();
	RenderTargetResource->ReadFloat16Pixels(FloatSurfaceData);

	// Create a new Texture2D with R16F format (single channel float)
	UTexture2D* NewTexture = UTexture2D::CreateTransient(RenderTarget->GetSurfaceWidth(), RenderTarget->GetSurfaceHeight(), PF_R16F);
	if (!NewTexture)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create transient texture with R16F format"));
		return nullptr;
	}

	// Lock the texture to get access to its raw data
	FTexture2DMipMap& Mip = NewTexture->GetPlatformData()->Mips[0];
	void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);

	// Copy only the alpha values from the render target to the R channel of the new texture
	for (int32 Index = 0; Index < FloatSurfaceData.Num(); ++Index)
	{
		// Each element of FloatSurfaceData is FFloat16Color, and we need only the Alpha channel
		FFloat16 AlphaValue = FloatSurfaceData[Index].A;  // Extract the Alpha channel as float16

		// Store the Alpha value in the R channel of the new texture
		FFloat16* DestPtr = (FFloat16*)TextureData + Index; // Pointer to the R channel of the texture
		*DestPtr = AlphaValue;
	}

	// Unlock and update the texture to apply changes
	Mip.BulkData.Unlock();
	NewTexture->UpdateResource();

	UE_LOG(LogTemp, Log, TEXT("Successfully created R16F texture from render target alpha"));

	return NewTexture;
}

void UFL_Render::CopyRenderTargetToRenderTargetByRenderThread(UTextureRenderTarget2D* SourceRenderTarget,
	UTextureRenderTarget2D* DestinationRenderTarget)
{
	if (!SourceRenderTarget || !DestinationRenderTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Source or Destination Render Target is null"));
		return;
	}

	// Retrieve the RHI textures from the source and destination render targets
	FRHITexture* SourceTextureRHI = SourceRenderTarget->GetResource()->TextureRHI;
	FRHITexture* DestinationTextureRHI = DestinationRenderTarget->GetResource()->TextureRHI;

	if (!SourceTextureRHI || !DestinationTextureRHI)
	{
		UE_LOG(LogTemp, Warning, TEXT("Source or Destination Texture RHI is null"));
		return;
	}

	// Enqueue render command to copy texture on the GPU
	ENQUEUE_RENDER_COMMAND(CopyRenderTargetCommand)(
		[SourceTextureRHI, DestinationTextureRHI](FRHICommandListImmediate& RHICmdList)
		{
			FRHICopyTextureInfo CopyInfo; // Default copy info to copy the whole texture
			RHICmdList.CopyTexture(SourceTextureRHI, DestinationTextureRHI, CopyInfo);
		}
	);
}

void UFL_Render::CopyRenderTargetToRenderTargetByCPU(UTextureRenderTarget2D* SourceRenderTarget,
	UTextureRenderTarget2D* DestinationRenderTarget)
{
	if (!SourceRenderTarget || !DestinationRenderTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Source or Destination Render Target is null"));
		return;
	}

	//DestinationRenderTarget->UpdateTexture2D(SourceRenderTarget)

	
}

FVector UFL_Render::GetWorldPositionFromRenderTarget(UTextureRenderTarget2D* RenderTarget, int32 X, int32 Y)
{
	if (!RenderTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Render Target is null"));
		return FVector::ZeroVector;
	}

	FTextureRenderTargetResource* RenderTargetResource = RenderTarget->GameThread_GetRenderTargetResource();
	if (!RenderTargetResource)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get render target resource"));
		return FVector::ZeroVector;
	}

	// Read a single pixel at (X, Y)
	TArray<FLinearColor> PixelData;
	FReadSurfaceDataFlags ReadPixelFlags(RCM_UNorm);
	ReadPixelFlags.SetLinearToGamma(false);

	// Get pixel data at the specified location
	if (RenderTargetResource->ReadLinearColorPixels(PixelData, ReadPixelFlags, FIntRect(X, Y, X + 1, Y + 1)))
	{
		if (PixelData.Num() > 0)
		{
			FLinearColor PixelColor = PixelData[0];
			// Convert color to 3D vector
			FVector WorldPosition(PixelColor.R, PixelColor.G, PixelColor.B);
			return WorldPosition;
		}
	}
    
	UE_LOG(LogTemp, Warning, TEXT("Failed to read pixel data from render target"));
	return FVector::ZeroVector;
}
