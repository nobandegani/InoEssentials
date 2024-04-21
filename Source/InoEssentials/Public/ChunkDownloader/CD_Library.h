/* Copyright (c) 2021-2023 by Inoland */

#pragma once

#include "ChunkDownloader.h"
#include "CD_Library.generated.h"

USTRUCT(BlueprintType, DisplayName="Chunk Downloader Stats", Category="Ino CD Stats")
struct FInoCdStats
{
    GENERATED_BODY()

    // number of pak files downloaded
    UPROPERTY(DisplayName="Files Downloaded")
        int FilesDownloaded = 0;

    UPROPERTY(DisplayName="Total Files To Download")
        int TotalFilesToDownload = 0;

    // number of bytes downloaded
    UPROPERTY(DisplayName="MegaBytes Downloaded")
        int MegaBytesDownloaded = 0;

    UPROPERTY(DisplayName="Total MegaBytes To Download")
        int TotalMegaBytesToDownload = 0;

    // number of chunks mounted (chunk is an ordered array of paks)
    UPROPERTY(DisplayName="Chunk Mounted")
        int ChunksMounted = 0;

    UPROPERTY(DisplayName="Total Chunks To Mount")
        int TotalChunksToMount = 0;

    // UTC time that loading began (for rate estimates)
    UPROPERTY(DisplayName="LoadingStartTime")
        FDateTime LoadingStartTime = FDateTime::MinValue();

    UPROPERTY(DisplayName="Last Error")
        FText LastError;

    FInoCdStats()
    {
        FilesDownloaded = 0;
        TotalFilesToDownload = 0;
        MegaBytesDownloaded = 0;
        TotalMegaBytesToDownload = 0;
        ChunksMounted = 0;
        TotalChunksToMount = 0;
        LoadingStartTime = FDateTime::MinValue();
        //LastError = "Unkown";
    }

    FInoCdStats(
        int InFilesDownloaded,
        int InTotalFilesToDownload,
        uint64 InBytesDownloaded,
        uint64 InTotalBytesToDownload,
        int InChunksMounted,
        int InTotalChunksToMount,
        FDateTime InLoadingStartTime,
        FText InLastError
        )
    {
        FilesDownloaded = InFilesDownloaded;
        TotalFilesToDownload = InTotalFilesToDownload;
        MegaBytesDownloaded = InBytesDownloaded;
        TotalMegaBytesToDownload = InTotalBytesToDownload;
        ChunksMounted = InChunksMounted;
        TotalChunksToMount = InTotalChunksToMount;
        LoadingStartTime = InLoadingStartTime;
        LastError = InLastError;
    }

    FInoCdStats(FChunkDownloader::FStats CdStats)
    {
        FilesDownloaded = CdStats.FilesDownloaded;
        TotalFilesToDownload = CdStats.TotalFilesToDownload;
        MegaBytesDownloaded = CdStats.BytesDownloaded * 0.000001;
        TotalMegaBytesToDownload = CdStats.TotalBytesToDownload * 0.000001;
        ChunksMounted = CdStats.ChunksMounted;
        TotalChunksToMount = CdStats.TotalChunksToMount;
        LoadingStartTime = CdStats.LoadingStartTime;
        LastError = CdStats.LastError;
    }
};