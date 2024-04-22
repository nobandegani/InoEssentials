/* Copyright (c) 2021-2024 by Inoland */

#include "Ino_WorldSubsystem.h"
#include "InoSubsystemTypes.h"

bool UIno_WorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && ReceiveShouldCreateSubsystem(Outer);
}

void UIno_WorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	check(!bInitialized)
	
	ReceiveInitialize();
	SetActive(bActiveDefault);
	
	bInitialized=true;
	
	UE_LOG(LogInoSubsystem, Log, TEXT("%s_Initialize"), *GetName())
}

void UIno_WorldSubsystem::Deinitialize()
{
	check(bInitialized)
	ReceiveDeinitialize();
	bInitialized=false;
	UE_LOG(LogInoSubsystem, Log, TEXT("%s_Deinitialize"), *GetName())
}

FPrimaryAssetId UIno_WorldSubsystem::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(PrimaryAssetType_InoSubsystem,GetFName());
}

bool UIno_WorldSubsystem::ReceiveShouldCreateSubsystem_Implementation(UObject* Outer) const
{
	return true;
}
