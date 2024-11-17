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

EPixelFormat InoReadRenderTargetHelper(TArray<FColor>& OutLDRValues, TArray<FLinearColor>& OutHDRValues,
	UObject* WorldContextObject, UTextureRenderTarget2D* TextureRenderTarget, int32 X, int32 Y, int32 Width,
	int32 Height, bool bNormalize)
{
	EPixelFormat OutFormat = PF_Unknown;

	if (!TextureRenderTarget)
	{
		return OutFormat;
	}

	FTextureRenderTarget2DResource* RTResource = (FTextureRenderTarget2DResource*)TextureRenderTarget->GameThread_GetRenderTargetResource();
	if (!RTResource)
	{
		return OutFormat;
	}

	X = FMath::Clamp(X, 0, TextureRenderTarget->SizeX - 1);
	Y = FMath::Clamp(Y, 0, TextureRenderTarget->SizeY - 1);
	Width = FMath::Clamp(Width, 1, TextureRenderTarget->SizeX);
	Height = FMath::Clamp(Height, 1, TextureRenderTarget->SizeY);
	Width = Width - FMath::Max(X + Width - TextureRenderTarget->SizeX, 0);
	Height = Height - FMath::Max(Y + Height - TextureRenderTarget->SizeY, 0);

	FIntRect SampleRect(X, Y, X + Width, Y + Height);

	FReadSurfaceDataFlags ReadSurfaceDataFlags = bNormalize ? FReadSurfaceDataFlags() : FReadSurfaceDataFlags(RCM_MinMax);

	FRenderTarget* RenderTarget = TextureRenderTarget->GameThread_GetRenderTargetResource();
	OutFormat = TextureRenderTarget->GetFormat();

	const int32 NumPixelsToRead = Width * Height;

	switch (OutFormat)
	{
	case PF_B8G8R8A8:
		if (!RenderTarget->ReadPixels(OutLDRValues, ReadSurfaceDataFlags, SampleRect))
		{
			OutFormat = PF_Unknown;
		}
		else
		{
			check(OutLDRValues.Num() == NumPixelsToRead);
		}
		break;
	case PF_FloatRGBA:
		if (!RenderTarget->ReadLinearColorPixels(OutHDRValues, ReadSurfaceDataFlags, SampleRect))
		{
			OutFormat = PF_Unknown;
		}
		else
		{
			check(OutHDRValues.Num() == NumPixelsToRead);
		}
		break;
	case PF_A32B32G32R32F:
		if (!RenderTarget->ReadLinearColorPixels(OutHDRValues, ReadSurfaceDataFlags, SampleRect))
		{
			OutFormat = PF_Unknown;
		}
		else
		{
			check(OutHDRValues.Num() == NumPixelsToRead);
		}
		break;
	default:
		OutFormat = PF_Unknown;
		break;
	}

	return OutFormat;
}

FLinearColor UFL_Render::ReadRawPixelFromRenderTarget(UObject* WorldContextObject, UTextureRenderTarget2D* TextureRenderTarget, int32 X, int32 Y, bool bNormalize)
{
	TArray<FColor> Samples;
	TArray<FLinearColor> LinearSamples;

	switch (InoReadRenderTargetHelper(Samples, LinearSamples, WorldContextObject, TextureRenderTarget, X, Y, 1, 1, bNormalize))
	{
	case PF_B8G8R8A8:
		check(Samples.Num() == 1 && LinearSamples.Num() == 0);
		return FLinearColor(float(Samples[0].R), float(Samples[0].G), float(Samples[0].B), float(Samples[0].A));
	case PF_FloatRGBA:
		check(Samples.Num() == 0 && LinearSamples.Num() == 1);
		return LinearSamples[0];
	case PF_A32B32G32R32F:
		check(Samples.Num() == 0 && LinearSamples.Num() == 1);
		return LinearSamples[0];
	case PF_Unknown:
	default:
		return FLinearColor::Red;
	}
}

bool UFL_Render::ReadRawPixelsFromRenderTarget(UObject* WorldContextObject, UTextureRenderTarget2D* TextureRenderTarget,
	TArray<FLinearColor>& OutLinearSamples, bool bNormalize)
{
	if (WorldContextObject != nullptr && TextureRenderTarget != nullptr)
	{
		const int32 NumSamples = TextureRenderTarget->SizeX * TextureRenderTarget->SizeY;

		OutLinearSamples.Reset(NumSamples);
		TArray<FColor> Samples;
		Samples.Reserve(NumSamples);

		switch (InoReadRenderTargetHelper(Samples, OutLinearSamples, WorldContextObject, TextureRenderTarget, 0, 0, TextureRenderTarget->SizeX, TextureRenderTarget->SizeY, bNormalize))
		{
		case PF_B8G8R8A8:
			check(Samples.Num() == NumSamples && OutLinearSamples.Num() == 0);
			for (int32 SampleIndex = 0; SampleIndex < NumSamples; ++SampleIndex)
			{
				OutLinearSamples.Add(FLinearColor(float(Samples[SampleIndex].R), float(Samples[SampleIndex].G), float(Samples[SampleIndex].B), float(Samples[SampleIndex].A)));
			}
			return true;
		case PF_FloatRGBA:
			check(Samples.Num() == 0 && OutLinearSamples.Num() == NumSamples);
			return true;
		case PF_A32B32G32R32F:
			check(Samples.Num() == 0 && OutLinearSamples.Num() == NumSamples);
			return true;
		case PF_Unknown:
		default:
			return false;
		}
	}

	return false;
}

bool UFL_Render::SavePixelsToFile(UObject* WorldContextObject, const TArray<FLinearColor> InLinearSamples,
	const FString& FilePath, const bool bOverride, const bool bCompress, EInoCompressor InCompressor,
	EInoCompressionLevel InCompressionLevel)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString Directory = FPaths::GetPath(FilePath);
	if (!PlatformFile.DirectoryExists(*Directory))
	{
		bool isValidPath = PlatformFile.CreateDirectory(*Directory);
		if (!isValidPath)
		{
			UE_LOG(LogTemp, Error, TEXT("directory dosnt exist and cant be created"));
			return false;
		}
	}
	
	if (InLinearSamples.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save: Input data is empty."));
		return false;
	}
	
	TArray<uint8> Data;
	
	if (bCompress)
	{
		UE_LOG(LogTemp, Log, TEXT("start compressing data"));
		TArray<uint8> CompressedData;
		FOodleDataCompression::ECompressor OodleCompressor = static_cast<FOodleDataCompression::ECompressor>(static_cast<int32>(InCompressor));
		FOodleDataCompression::ECompressionLevel CompressionLevel = static_cast<FOodleDataCompression::ECompressionLevel>((static_cast<int32>(InCompressionLevel) -4));
		UE_LOG(LogTemp, Log, TEXT("Compressor: %d, Compression Level: %d"), static_cast<int32>(OodleCompressor), static_cast<int32>(CompressionLevel));

		int32 MaxCompressedSize = FOodleDataCompression::GetMaximumCompressedSize(InLinearSamples.Num());
		MaxCompressedSize += MaxCompressedSize / 2;
		CompressedData.SetNumUninitialized(MaxCompressedSize);

		int32 CompressedSize = FOodleDataCompression::Compress(
					CompressedData.GetData(),
					MaxCompressedSize,
					InLinearSamples.GetData(), //reinterpret_cast<const uint8*>(InLinearSamples.GetData()),
					InLinearSamples.Num(),
					OodleCompressor,
					CompressionLevel
					);
		
		if (CompressedSize > 0)
		{
			CompressedData.SetNum(CompressedSize);
			Data = CompressedData;
			CompressedData.Empty();
			UE_LOG(LogTemp, Log, TEXT("Compression successful!"));
		}else
		{
			UE_LOG(LogTemp, Error, TEXT("Compression failed! CompressedSize returned as 0 or less."));
			CompressedData.Empty();
			return false;
		}
	}else
	{
		Data.Append(reinterpret_cast<const uint8*>(InLinearSamples.GetData()), InLinearSamples.Num() * sizeof(FLinearColor));
	}
	
	UE_LOG(LogTemp, Log, TEXT("start saving file"));

	if (PlatformFile.FileExists(*FilePath) && !bOverride)
	{
		UE_LOG(LogTemp, Error, TEXT("File already exists and override is not allowed."));
		return false;
	}

	if (FFileHelper::SaveArrayToFile(Data, *FilePath))
	{
		UE_LOG(LogTemp, Log, TEXT("File saved successfully: %s"), *FilePath);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save file: %s"), *FilePath);
		return false;
	}
	
}

