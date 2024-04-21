/* Copyright (c) 2021-2023 by Inoland */


#include "Subsystems/SS_GI.h"

DEFINE_LOG_CATEGORY(InoSSGI);

void UInoGISS::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	UE_LOG(InoSSGI, Warning, TEXT("%s Initialized"), *GetName());

	if (AutomaticWorldContext)
	{
		FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UInoGISS::OnPostWorldInit);
		
		SetWorldContext(GetGameInstance());
	}
	
	Init();
}

void UInoGISS::Deinitialize()
{
	UE_LOG(InoSSGI, Warning, TEXT("%s Deinitialized"), *GetName());
	
	DeInit();
	
	Super::Deinitialize();
}

void UInoGISS::OnLevelChanged(ULevel* Level, UWorld* World)
{
	//WorldUpdated();
}

void UInoGISS::OnPostWorldInit(UWorld* World, const UWorld::InitializationValues Values)
{
	if (AutomaticWorldContext){
		if (WorldContext != nullptr)
		{
			delete WorldContext;
			WorldContext = nullptr;
		}
		
		WorldContext = new FWorldContext();
		WorldContext->SetCurrentWorld(World);

		WorldUpdated();
	}
}

void UInoGISS::ForceInit()
{
	UE_LOG(InoSSGI, Warning, TEXT("%s force init called"), *GetName());
}

void UInoGISS::SetWorldContext(const UObject* NewWorldContext)
{
	if (NewWorldContext == nullptr)
	{
		return;
	}
	if (NewWorldContext->GetWorld() == nullptr)
	{
		return;
	}

	bool useinit = false;
	if (WorldContext == nullptr)
	{
		useinit = true;
	}else
	{
		delete WorldContext;
		WorldContext = nullptr;
	}
	
	WorldContext = new FWorldContext();
	WorldContext->SetCurrentWorld(NewWorldContext->GetWorld());
	
	if (useinit)
	{
		WorldInit();
	}else
	{
		WorldUpdated();
	}
}

UObject* UInoGISS::GetWorldContextObject()
{
	if (WorldContext == nullptr)
	{
		return nullptr;
	}
	if (WorldContext->World() == nullptr)
	{
		return nullptr;
	}
	
	return WorldContext->World();
}

class UWorld* UInoGISS::GetWorld() const
{
	return WorldContext ? WorldContext->World() : nullptr;
}