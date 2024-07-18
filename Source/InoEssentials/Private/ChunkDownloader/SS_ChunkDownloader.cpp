/* Copyright (c) 2021-2023 by Inoland */

#include "ChunkDownloader/SS_ChunkDownloader.h"

#include "ChunkDownloader.h"
#include "Misc/CoreDelegates.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Async/Async.h"
#include "Serialization/JsonSerializerMacros.h"

#define Ino_Thread_NormalAsync	ENamedThreads::AnyHiPriThreadNormalTask
#define Ino_Thread_Normal	ENamedThreads::GameThread


DEFINE_LOG_CATEGORY_STATIC(ChunkDownloaderSubsystem, Display, Display);

void UChunkDownloaderSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UChunkDownloaderSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
	FChunkDownloader::Shutdown();
}

void UChunkDownloaderSubsystem::Init(FString PlatformName, FString DeploymentName, FString ContentBuildId, const F_CD_D_Initilized Callback)
{
	if (IsPlayInEditor())
	{
		Callback.ExecuteIfBound(false,false);
		return;
	}
	
	AsyncTask(Ino_Thread_NormalAsync, [PlatformName, DeploymentName, ContentBuildId, Callback] ()
	{
		TSharedRef<FChunkDownloader> CDRef = FChunkDownloader::GetOrCreate();
		
		// initialize the chunk downloader with chosen platform
		CDRef->Initialize(PlatformName, 8);
		UE_LOG(ChunkDownloaderSubsystem, Display, TEXT("Initilizing ChunkDownloader"));
		
		// load the cached build ID
		bool LoadCachedBuild = CDRef->LoadCachedBuild(DeploymentName);
		
		// update the build manifest file
		TFunction<void(bool bSuccess)> UpdateCompleteCallback = [&](bool bSuccess)
		{
			AsyncTask(Ino_Thread_Normal, [bSuccess, LoadCachedBuild, Callback] ()
			{
				Callback.ExecuteIfBound(LoadCachedBuild, bSuccess);
				UE_LOG(ChunkDownloaderSubsystem, Display, TEXT("Update Build complete, Cached %d, update %d"), LoadCachedBuild, bSuccess);
			});
		};
		CDRef->UpdateBuild(DeploymentName, ContentBuildId, UpdateCompleteCallback); 
	});
}

FInoCdStats UChunkDownloaderSubsystem::GetLoadingStats()
{
	//Get a reference to ChunkDownloader
	TSharedRef<FChunkDownloader> Downloader = FChunkDownloader::GetChecked();

	//Get the loading stats struct
	FChunkDownloader::FStats LoadingStats = Downloader->GetLoadingStats();

	return FInoCdStats(LoadingStats);
}

bool UChunkDownloaderSubsystem::IsPlayInEditor()
{
	return true;//GetWorld()->IsPlayInEditor();
}
