/* Copyright (c) 2021-2023 by Inoland */

#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "ChunkDownloader/CD_Library.h"

#include "SS_ChunkDownloader.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(F_CD_D_Initilized, bool, LoadCachedBuild, bool, UpdateBuild);

UCLASS( DisplayName="Chunk Downloader Subsystem", Category="Ino CD SS")
class UChunkDownloaderSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable, DisplayName="Initilize", Category="Ino CD SS")
		void Init(FString PlatformName, FString DeploymentName, FString ContentBuildId, const F_CD_D_Initilized Callback);

	UFUNCTION(BlueprintCallable, DisplayName="Get Loading Stats", Category="Ino CD SS")
		FInoCdStats GetLoadingStats();

protected:
	UFUNCTION()
		bool IsPlayInEditor();
};