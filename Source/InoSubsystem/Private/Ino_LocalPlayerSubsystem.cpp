/* Copyright (c) 2021-2024 by Inoland */

#include "Ino_LocalPlayerSubsystem.h"
#include "InoSubsystem/InoSubsystemTypes.h"

void UIno_LocalPlayerSubsystem::PostWorldInitialization(UWorld* World, const UWorld::InitializationValues IVS)
{
	World->OnWorldBeginPlay.AddUObject(this, &UIno_LocalPlayerSubsystem::WorldBeginPlay);
}

void UIno_LocalPlayerSubsystem::StartGameInstance(UGameInstance* GameInstance)
{
	if (GWorld)
	{
		GWorld->OnWorldBeginPlay.AddUObject(this, &UIno_LocalPlayerSubsystem::WorldBeginPlay);
	}
}

bool UIno_LocalPlayerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && ReceiveShouldCreateSubsystem(Outer);
}

void UIno_LocalPlayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	check(!bInitialized)

	// World delegation assign

	FWorldDelegates::OnStartGameInstance.AddUObject(this, &UIno_LocalPlayerSubsystem::StartGameInstance);
	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UIno_LocalPlayerSubsystem::PostWorldInitialization);

	FWorldDelegates::OnWorldBeginTearDown.AddUObject(this, &UIno_LocalPlayerSubsystem::WorldBeginTearingDown);

	ReceiveInitialize();
	SetActive(bActiveDefault);

	bInitialized = true;

	UE_LOG(LogInoSubsystem, Log, TEXT("%s_Initialize"), *GetName())
}

void UIno_LocalPlayerSubsystem::Deinitialize()
{
	check(bInitialized);
	ReceiveDeinitialize();
	bInitialized = false;
	UE_LOG(LogInoSubsystem, Log, TEXT("%s_Deinitialize"), *GetName())
}

FPrimaryAssetId UIno_LocalPlayerSubsystem::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(PrimaryAssetType_InoSubsystem, GetFName());
}

bool UIno_LocalPlayerSubsystem::ReceiveShouldCreateSubsystem_Implementation(UObject* Outer) const
{
	return true;
}
