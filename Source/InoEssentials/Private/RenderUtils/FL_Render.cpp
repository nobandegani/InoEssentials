/* Copyright (c) 2021-2023 by Inoland */

#include "RenderUtils/FL_Render.h"

#include "FunctionLibraries/FL_Extra.h"

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

EPixelFormat InoReadRenderTargetHelper(
	TArray<FColor>& Out8Bit,
	TArray<FFloat16Color>& Out16Bit,
	TArray<FLinearColor>& Out32Bit,
	UObject* WorldContextObject,
	UTextureRenderTarget2D* TextureRenderTarget,
	int32 X,
	int32 Y,
	int32 Width,
	int32 Height,
	bool bNormalize)
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
		if (!RenderTarget->ReadPixels(Out8Bit, ReadSurfaceDataFlags, SampleRect))
		{
			OutFormat = PF_Unknown;
		}
		else
		{
			check(Out8Bit.Num() == NumPixelsToRead);
		}
		break;
	case PF_FloatRGBA:
		if (!RenderTarget->ReadFloat16Pixels(Out16Bit, ReadSurfaceDataFlags, SampleRect))
		{
			OutFormat = PF_Unknown;
		}
		else
		{
			check(Out16Bit.Num() == NumPixelsToRead);
		}
		break;
	case PF_A32B32G32R32F:
		if (!RenderTarget->ReadLinearColorPixels(Out32Bit, ReadSurfaceDataFlags, SampleRect))
		{
			OutFormat = PF_Unknown;
		}
		else
		{
			check(Out32Bit.Num() == NumPixelsToRead);
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
	TArray<FColor> Samples8Bit;;
	TArray<FFloat16Color> Samples16Bit;
	TArray<FLinearColor> Samples32Bit;

	switch (InoReadRenderTargetHelper(Samples8Bit, Samples16Bit, Samples32Bit, WorldContextObject, TextureRenderTarget, X, Y, 1, 1, bNormalize))
	{
	case PF_B8G8R8A8:
		check(Samples8Bit.Num() == 1 && Samples16Bit.Num() == 0 && Samples32Bit.Num() == 0);
		return FLinearColor(float(Samples8Bit[0].R), float(Samples8Bit[0].G), float(Samples8Bit[0].B), float(Samples8Bit[0].A));
	case PF_FloatRGBA:
		check(Samples8Bit.Num() == 0 && Samples16Bit.Num() == 1 && Samples32Bit.Num() == 0);
		return FLinearColor(float(Samples16Bit[0].R), float(Samples16Bit[0].G), float(Samples16Bit[0].B), float(Samples16Bit[0].A));
	case PF_A32B32G32R32F:
		check(Samples8Bit.Num() == 0 && Samples16Bit.Num() == 0 && Samples32Bit.Num() == 1);
		return Samples32Bit[0];
	case PF_Unknown:
	default:
		return FLinearColor::Red;
	}
}

bool UFL_Render::ReadRawPixelsFromRenderTarget(
	UObject* WorldContextObject,
	UTextureRenderTarget2D* TextureRenderTarget,
	TArray<FLinearColor>& OutSamples,
	bool bNormalize)
{
	if (WorldContextObject != nullptr && TextureRenderTarget != nullptr)
	{
		const int32 NumSamples = TextureRenderTarget->SizeX * TextureRenderTarget->SizeY;

		OutSamples.Reset(NumSamples);
		
		TArray<FColor> Out8BitSamples;
		TArray<FFloat16Color> Out16BitSamples;
		Out8BitSamples.Reserve(NumSamples);
		Out16BitSamples.Reserve(NumSamples);
		
		switch (InoReadRenderTargetHelper(Out8BitSamples, Out16BitSamples, OutSamples, WorldContextObject, TextureRenderTarget, 0, 0, TextureRenderTarget->SizeX, TextureRenderTarget->SizeY, bNormalize))
		{
		case PF_B8G8R8A8:
			check(Out8BitSamples.Num() == NumSamples && Out16BitSamples.Num() == 0 && OutSamples.Num() == 0);
			for (int32 SampleIndex = 0; SampleIndex < NumSamples; ++SampleIndex)
			{
				Out8BitSamples.Add(Out8BitSamples[SampleIndex]);
			}
			return true;
		case PF_FloatRGBA:
			check(Out8BitSamples.Num() == 0 && Out16BitSamples.Num() == NumSamples && OutSamples.Num() == 0);
			for (int32 SampleIndex = 0; SampleIndex < NumSamples; ++SampleIndex)
			{
				Out16BitSamples.Add(Out16BitSamples[SampleIndex]);
			}
			return true;
		case PF_A32B32G32R32F:
			check(Out8BitSamples.Num() == 0 && Out16BitSamples.Num() == 0 && OutSamples.Num() == NumSamples);
			return true;
		case PF_Unknown:
		default:
			return false;
		}
	}
	
	return false;
}

bool UFL_Render::SavePixelsToFile(
	const TArray<FLinearColor>& InLinearSamples,
	const FString& InFilePath,
	const bool bOverride,
	const bool bCompress,
	EInoCompressor InCompressor,
	EInoCompressionLevel InCompressionLevel,
	EInoDataType DataType,
	EInoChannelType ChannelType,
	FString& OutFilePath)
{
	if (InFilePath.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save: FilePath is empty."));
		return false;
	}
	
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString Directory = FPaths::GetPath(OutFilePath);
	if (!PlatformFile.DirectoryExists(*Directory))
	{
		if (!PlatformFile.CreateDirectory(*Directory))
		{
			UE_LOG(LogTemp, Error, TEXT("Directory doesn't exist and couldn't be created: %s"), *Directory);
			return false;
		}
	}
	
	if (InLinearSamples.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save: Input data is empty."));
		return false;
	}

	TArray<uint8> InputData;

	switch (DataType)
	{
	case EInoDataType::Bit8:
		if (ChannelType == EInoChannelType::RGBA)
		{
			UE_LOG(LogTemp, Log, TEXT("not supported data type"));
			return false;
		}else if (ChannelType == EInoChannelType::RGB)
		{
			UE_LOG(LogTemp, Log, TEXT("not supported data type"));
			return false;
		}else if (ChannelType == EInoChannelType::RG || ChannelType == EInoChannelType::RB || ChannelType == EInoChannelType::GB || ChannelType == EInoChannelType::RA || ChannelType == EInoChannelType::GA || ChannelType == EInoChannelType::BA)
		{
			UE_LOG(LogTemp, Log, TEXT("not supported data type"));
			return false;
		}else
		{
			UE_LOG(LogTemp, Log, TEXT("not supported data type"));
			return false;
		}
			break;
	case EInoDataType::Bit16:
		if (ChannelType == EInoChannelType::RGBA)
		{
			TArray<FFloat16Color> TempArray = ConvertLinearColorToFloat16Color(InLinearSamples);
			InputData.Append(reinterpret_cast<const uint8*>(TempArray.GetData()), TempArray.NumBytes());
		}else if (ChannelType == EInoChannelType::RGB)
		{
			UE_LOG(LogTemp, Log, TEXT("not supported data type"));
			return false;
		}else if (ChannelType == EInoChannelType::RG || ChannelType == EInoChannelType::RB || ChannelType == EInoChannelType::GB || ChannelType == EInoChannelType::RA || ChannelType == EInoChannelType::GA || ChannelType == EInoChannelType::BA)
		{
			UE_LOG(LogTemp, Log, TEXT("not supported data type"));
			return false;
		}else
		{
			UE_LOG(LogTemp, Log, TEXT("not supported data type"));
			return false;
		}
		break;
	case EInoDataType::Bit32:
		if (ChannelType == EInoChannelType::RGBA)
		{
			InputData.Append(reinterpret_cast<const uint8*>(InLinearSamples.GetData()), InLinearSamples.NumBytes());
		}else if (ChannelType == EInoChannelType::RGB)
		{
			TArray<FVector3f> TempArray = ConvertLinearColorToVector3f(InLinearSamples);
			InputData.Append(reinterpret_cast<const uint8*>(TempArray.GetData()), TempArray.NumBytes());
		}else if (ChannelType == EInoChannelType::RG || ChannelType == EInoChannelType::RB || ChannelType == EInoChannelType::GB || ChannelType == EInoChannelType::RA || ChannelType == EInoChannelType::GA || ChannelType == EInoChannelType::BA)
		{
			UE_LOG(LogTemp, Log, TEXT("not supported data type"));
			return false;
		}else
		{
			UE_LOG(LogTemp, Log, TEXT("not supported data type"));
			return false;
		}
		break;
	case EInoDataType::Bit64:
		UE_LOG(LogTemp, Log, TEXT("not supported data type"));
		return false;
	default:
		UE_LOG(LogTemp, Log, TEXT("not supported data type"));
		return false;
	}

	
	
	TArray<uint8> OutputData;
	
	int64 UnCompressedSize = InputData.NumBytes();

	if (bCompress)
	{
		int64 DeCompressedSize;
		UFL_Extra::CompressDataWithOodle(InputData, InCompressor, InCompressionLevel, DeCompressedSize, OutputData);
	}else
	{
		UE_LOG(LogTemp, Log, TEXT("Skipping compression, saving raw data."));
		OutputData = InputData;
	}
	
	UE_LOG(LogTemp, Log, TEXT("start saving file"));

	FString CompressExten = bCompress ? "-C" : "-U";
	OutFilePath = InFilePath + CompressExten + FString::Printf(TEXT("-%lld"), UnCompressedSize) + TEXT(".InoR") + FileExtenstionByDataType(DataType, ChannelType);
	if (PlatformFile.FileExists(*OutFilePath) && !bOverride)
	{
		UE_LOG(LogTemp, Error, TEXT("File already exists and override is not allowed."));
		return false;
	}

	if (FFileHelper::SaveArrayToFile(OutputData, *OutFilePath))
	{
		UE_LOG(LogTemp, Log, TEXT("File saved successfully: %s"), *OutFilePath);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save file: %s"), *OutFilePath);
		return false;
	}
}

bool UFL_Render::LoadPixelsFromFile(
	const FString& FilePath,
	const bool bDeCompress,
	TArray<FLinearColor>& OutLinearSamples)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.FileExists(*FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("File doesn't exist: %s"), *FilePath);
		return false;
	}

	TArray<uint8> Data;
	if (!FFileHelper::LoadFileToArray(Data, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("File can't be loaded: %s"), *FilePath);
		return false;
	}

	if (Data.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Data did not load correctly from file: %s"), *FilePath);
		return false;
	}

	if (bDeCompress)
	{
		FString FileName = FPaths::GetCleanFilename(FilePath);
		FString BaseFileName, SizeString;
		if (!FileName.Split(TEXT("-"), &BaseFileName, &SizeString, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to parse uncompressed size from file name: %s"), *FileName);
			return false;
		}

		SizeString = FPaths::GetBaseFilename(SizeString);
		int32 DeCompressedSize = FCString::Atoi64(*SizeString);

		if (DeCompressedSize <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid uncompressed size parsed from file name: %s"), *SizeString);
			return false;
		}
		TArray<uint8> DecompressedData;
		if (!UFL_Extra::DecompressDataWithOodle(Data, DeCompressedSize, DecompressedData))
		{
			UE_LOG(LogTemp, Error, TEXT("Decompress failed."));
			return false;
		}
		
		Data.Empty();
		Data = DecompressedData;
		DecompressedData.Empty();
	}
	
	if (!DeserializeLinearColorArray(Data, OutLinearSamples))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to deserialize raw data."));
		return false;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Successfully loaded and processed pixel data from file: %s"), *FilePath);
	return true;
}

bool UFL_Render::SaveRenderTargetToFileAsData(
	UObject* WorldContextObject,
	UTextureRenderTarget2D* TextureRenderTarget,
	bool bNormalize,
	const FString& InFilePath,
	const bool bOverride,
	const bool bCompress,
	const EInoCompressor InCompressor,
	const EInoCompressionLevel InCompressionLevel,
	const EInoChannelType ChannelType,
	FString& OutFilePath)
{
	if (WorldContextObject == nullptr || TextureRenderTarget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Render context is null."));
		return false;
	}

	if (InFilePath.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save: FilePath is empty."));
		return false;
	}
	
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString Directory = FPaths::GetPath(OutFilePath);
	if (!PlatformFile.DirectoryExists(*Directory))
	{
		if (!PlatformFile.CreateDirectory(*Directory))
		{
			UE_LOG(LogTemp, Error, TEXT("Directory doesn't exist and couldn't be created: %s"), *Directory);
			return false;
		}
	}
	
	const int32 NumSamples = TextureRenderTarget->SizeX * TextureRenderTarget->SizeY;
	
	TArray<FColor> Out8BitSamples;
	TArray<FFloat16Color> Out16BitSamples;
	TArray<FLinearColor> Out32BitSamples;
	Out8BitSamples.Reserve(NumSamples);
	Out16BitSamples.Reserve(NumSamples);
	Out32BitSamples.Reserve(NumSamples);

	EPixelFormat RTPixelFormat = InoReadRenderTargetHelper(Out8BitSamples, Out16BitSamples, Out32BitSamples, WorldContextObject, TextureRenderTarget, 0, 0, TextureRenderTarget->SizeX, TextureRenderTarget->SizeY, bNormalize);
	
	switch (RTPixelFormat)
	{
	case PF_B8G8R8A8:
		check(Out8BitSamples.Num() == NumSamples && Out16BitSamples.Num() == 0 && Out32BitSamples.Num() == 0);
		break;
	case PF_FloatRGBA:
		check(Out8BitSamples.Num() == 0 && Out16BitSamples.Num() == NumSamples && Out32BitSamples.Num() == 0);
		break;
	case PF_A32B32G32R32F:
		check(Out8BitSamples.Num() == 0 && Out16BitSamples.Num() == 0 && Out32BitSamples.Num() == NumSamples);
		break;
	case PF_Unknown:
		UE_LOG(LogTemp, Error, TEXT("Failed to save: unknown pixel format"));
		return false;
	default:
		UE_LOG(LogTemp, Error, TEXT("Failed to save: unknown pixel format"));
		return false;
	}

	TArray<uint8> InputData;

	switch (RTPixelFormat)
	{
	case PF_B8G8R8A8:
		if (ChannelType == EInoChannelType::RGBA)
		{
			InputData.Append(reinterpret_cast<const uint8*>(Out8BitSamples.GetData()), Out8BitSamples.NumBytes());
		}else if (ChannelType == EInoChannelType::RGB)
		{
			UE_LOG(LogTemp, Log, TEXT("not supported data type"));
			return false;
		}else
		{
			UE_LOG(LogTemp, Log, TEXT("not supported data type"));
			return false;
		}
			break;
	case PF_FloatRGBA:
		if (ChannelType == EInoChannelType::RGBA)
		{
			InputData.Append(reinterpret_cast<const uint8*>(Out16BitSamples.GetData()), Out16BitSamples.NumBytes());
		}else if (ChannelType == EInoChannelType::RGB)
		{
			UE_LOG(LogTemp, Log, TEXT("not supported data type"));
			return false;
		}else
		{
			UE_LOG(LogTemp, Log, TEXT("not supported data type"));
			return false;
		}
		break;
	case PF_A32B32G32R32F:
		if (ChannelType == EInoChannelType::RGBA)
		{
			InputData.Append(reinterpret_cast<const uint8*>(Out32BitSamples.GetData()), Out32BitSamples.NumBytes());
		}else if (ChannelType == EInoChannelType::RGB)
		{
			TArray<FVector3f> TempArray = ConvertLinearColorToVector3f(Out32BitSamples);
			InputData.Append(reinterpret_cast<const uint8*>(TempArray.GetData()), TempArray.NumBytes());
		}else
		{
			UE_LOG(LogTemp, Log, TEXT("not supported data type"));
			return false;
		}
		break;
	default:
		UE_LOG(LogTemp, Log, TEXT("not supported data type"));
		return false;
	}
	
	TArray<uint8> OutputData;
	
	int64 UnCompressedSize = InputData.NumBytes();

	if (bCompress)
	{
		UFL_Extra::CompressDataWithOodle(InputData, InCompressor, InCompressionLevel, UnCompressedSize, OutputData);
	}else
	{
		UE_LOG(LogTemp, Log, TEXT("Skipping compression, saving raw data."));
		OutputData = InputData;
	}
	
	UE_LOG(LogTemp, Log, TEXT("start saving file"));

	FString CompressExten = bCompress ? "-C" : "-U";
	OutFilePath = InFilePath + CompressExten + FString::Printf(TEXT("-%lld"), UnCompressedSize) + TEXT(".InoR") + FileExtenstionByPixelFormat(RTPixelFormat, ChannelType);
	if (PlatformFile.FileExists(*OutFilePath) && !bOverride)
	{
		UE_LOG(LogTemp, Error, TEXT("File already exists and override is not allowed."));
		return false;
	}

	if (FFileHelper::SaveArrayToFile(OutputData, *OutFilePath))
	{
		UE_LOG(LogTemp, Log, TEXT("File saved successfully: %s"), *OutFilePath);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save file: %s"), *OutFilePath);
		return false;
	}
}

bool UFL_Render::DeserializeLinearColorArray(const TArray<uint8>& Data, TArray<FLinearColor>& OutLinearSamples)
{
	int64 NumColors = Data.Num() / sizeof(FLinearColor);

	if (NumColors * sizeof(FLinearColor) != Data.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Data size is not a multiple of FLinearColor size. Corrupted or invalid data."));
		return false;
	}

	OutLinearSamples.SetNumUninitialized(NumColors);

	FMemory::Memcpy(OutLinearSamples.GetData(), Data.GetData(), Data.Num());
	return true;
}

TArray<FVector3f> UFL_Render::ConvertLinearColorToVector3f(const TArray<FLinearColor>& InLinearSamples)
{
	TArray<FVector3f> OutVectorSamples;
	OutVectorSamples.Reserve(InLinearSamples.Num());
	
	for (const FLinearColor& Color : InLinearSamples)
	{
		FVector3f VectorSample(Color.R, Color.G, Color.B);
		OutVectorSamples.Add(VectorSample);
	}

	return OutVectorSamples;
}

TArray<FVector> UFL_Render::ConvertLinearColorToVector(const TArray<FLinearColor>& InLinearSamples)
{
	TArray<FVector> OutVectorSamples;
	OutVectorSamples.Reserve(InLinearSamples.Num());

	for (const FLinearColor& Color : InLinearSamples)
	{
		FVector VectorSample(Color.R, Color.G, Color.B);
		OutVectorSamples.Add(VectorSample);
	}

	return OutVectorSamples;
}

FString UFL_Render::FileExtenstionByDataType(const EInoDataType DataType, const EInoChannelType ChannelType)
{
	FString DataTypeExten;
	switch (DataType)
	{
	case EInoDataType::Bit8:
		if (ChannelType == EInoChannelType::RGBA)
		{
			DataTypeExten = "4f08b";
		}else if (ChannelType == EInoChannelType::RGB)
		{
			DataTypeExten = "3f08b";
		}else if (ChannelType == EInoChannelType::RG || ChannelType == EInoChannelType::RB || ChannelType == EInoChannelType::GB || ChannelType == EInoChannelType::RA || ChannelType == EInoChannelType::GA || ChannelType == EInoChannelType::BA)
		{
			DataTypeExten = "2f08b";
		}else
		{
			DataTypeExten = "1f08b";
		}
		break;
	case EInoDataType::Bit16:
		if (ChannelType == EInoChannelType::RGBA)
		{
			DataTypeExten = "4f16b";
		}else if (ChannelType == EInoChannelType::RGB)
		{
			DataTypeExten = "3f16b";
		}else if (ChannelType == EInoChannelType::RG || ChannelType == EInoChannelType::RB || ChannelType == EInoChannelType::GB || ChannelType == EInoChannelType::RA || ChannelType == EInoChannelType::GA || ChannelType == EInoChannelType::BA)
		{
			DataTypeExten = "2f16b";
		}else
		{
			DataTypeExten = "1f16b";
		}
		break;
	case EInoDataType::Bit32:
		if (ChannelType == EInoChannelType::RGBA)
		{
			DataTypeExten = "4f32b";
		}else if (ChannelType == EInoChannelType::RGB)
		{
			DataTypeExten = "3f32b";
		}else if (ChannelType == EInoChannelType::RG || ChannelType == EInoChannelType::RB || ChannelType == EInoChannelType::GB || ChannelType == EInoChannelType::RA || ChannelType == EInoChannelType::GA || ChannelType == EInoChannelType::BA)
		{
			DataTypeExten = "2f32b";
		}else
		{
			DataTypeExten = "1f32b";
		}
		break;
	case EInoDataType::Bit64:
		if (ChannelType == EInoChannelType::RGBA)
		{
			DataTypeExten = "4f64b";
		}else if (ChannelType == EInoChannelType::RGB)
		{
			DataTypeExten = "3f64b";
		}else if (ChannelType == EInoChannelType::RG || ChannelType == EInoChannelType::RB || ChannelType == EInoChannelType::GB || ChannelType == EInoChannelType::RA || ChannelType == EInoChannelType::GA || ChannelType == EInoChannelType::BA)
		{
			DataTypeExten = "2f64b";
		}else
		{
			DataTypeExten = "1f64b";
		}
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("unsupported data type"));
		DataTypeExten = "unkown";
		break;
	}
	
	return DataTypeExten;
}

FString UFL_Render::FileExtenstionByPixelFormat(const EPixelFormat PixelFormat, const EInoChannelType ChannelType)
{
	FString DataTypeExten;
	switch (PixelFormat)
	{
	case PF_B8G8R8A8:
		if (ChannelType == EInoChannelType::RGBA)
		{
			DataTypeExten = "4f08b";
		}else if (ChannelType == EInoChannelType::RGB)
		{
			DataTypeExten = "3f08b";
		}else if (ChannelType == EInoChannelType::RG || ChannelType == EInoChannelType::RB || ChannelType == EInoChannelType::GB || ChannelType == EInoChannelType::RA || ChannelType == EInoChannelType::GA || ChannelType == EInoChannelType::BA)
		{
			DataTypeExten = "2f08b";
		}else
		{
			DataTypeExten = "1f08b";
		}
		break;
	case PF_FloatRGBA:
		if (ChannelType == EInoChannelType::RGBA)
		{
			DataTypeExten = "4f16b";
		}else if (ChannelType == EInoChannelType::RGB)
		{
			DataTypeExten = "3f16b";
		}else if (ChannelType == EInoChannelType::RG || ChannelType == EInoChannelType::RB || ChannelType == EInoChannelType::GB || ChannelType == EInoChannelType::RA || ChannelType == EInoChannelType::GA || ChannelType == EInoChannelType::BA)
		{
			DataTypeExten = "2f16b";
		}else
		{
			DataTypeExten = "1f16b";
		}
		break;
	case PF_A32B32G32R32F:
		if (ChannelType == EInoChannelType::RGBA)
		{
			DataTypeExten = "4f32b";
		}else if (ChannelType == EInoChannelType::RGB)
		{
			DataTypeExten = "3f32b";
		}else if (ChannelType == EInoChannelType::RG || ChannelType == EInoChannelType::RB || ChannelType == EInoChannelType::GB || ChannelType == EInoChannelType::RA || ChannelType == EInoChannelType::GA || ChannelType == EInoChannelType::BA)
		{
			DataTypeExten = "2f32b";
		}else
		{
			DataTypeExten = "1f32b";
		}
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("unsupported data type"));
		DataTypeExten = "unkown";
		break;
	}
	
	return DataTypeExten;
}

TArray<FFloat16Color> UFL_Render::ConvertLinearColorToFloat16Color(const TArray<FLinearColor>& InLinearSamples)
{
	TArray<FFloat16Color> OutFloat16ColorSamples;
	OutFloat16ColorSamples.Reserve(InLinearSamples.Num());

	for (const FLinearColor& Color : InLinearSamples)
	{
		FFloat16Color Float16ColorSample(Color);
		OutFloat16ColorSamples.Add(Float16ColorSample);
	}

	return OutFloat16ColorSamples;
}
