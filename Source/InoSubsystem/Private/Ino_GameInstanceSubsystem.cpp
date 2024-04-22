/* Copyright (c) 2021-2024 by Inoland */

#include "Ino_GameInstanceSubsystem.h"
#include "InoSubsystemTypes.h"

void UIno_GameInstanceSubsystem::PostWorldInitialization(UWorld* World, const UWorld::InitializationValues IVS)
{
	World->OnWorldBeginPlay.AddUObject(this, &UIno_GameInstanceSubsystem::WorldBeginPlay);
}

void UIno_GameInstanceSubsystem::StartGameInstance(UGameInstance* GameInstance)
{
	if (GWorld)
	{
		GWorld->OnWorldBeginPlay.AddUObject(this, &UIno_GameInstanceSubsystem::WorldBeginPlay);
	}
}

bool UIno_GameInstanceSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && ReceiveShouldCreateSubsystem(Outer);
}

void UIno_GameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	check(!bInitialized)
	
	FWorldDelegates::OnPostWorldInitialization.
		AddUObject(this, &UIno_GameInstanceSubsystem::PostWorldInitialization);
	FWorldDelegates::OnWorldBeginTearDown.AddUObject(this,&UIno_GameInstanceSubsystem::WorldBeginTearingDown);

	ReceiveInitialize();
	SetActive(bActiveDefault);

	bInitialized = true;

	UE_LOG(LogInoSubsystem, Log, TEXT("%s_Initialize"), *GetName())
}

void UIno_GameInstanceSubsystem::Deinitialize()
{
	check(bInitialized)
	ReceiveDeinitialize();
	bInitialized = false;
	UE_LOG(LogInoSubsystem, Log, TEXT("%s_Deinitialize"), *GetName())
}

FPrimaryAssetId UIno_GameInstanceSubsystem::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(PrimaryAssetType_InoSubsystem,GetFName());
}

bool UIno_GameInstanceSubsystem::ReceiveShouldCreateSubsystem_Implementation(UObject* Outer) const
{
	return true;
}
