// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/FL_SS.h"

bool UFL_SS::AddInoGISubsystem(UGameInstance* GameInstance, TSubclassOf<UInoGISS> InoGISSSubclass)
{
	if (!GameInstance)
	{
		return false;
	}
	
	UInoGISS* InoGISSInstance = NewObject<UInoGISS>(GameInstance, InoGISSSubclass);
	if(InoGISSInstance)
	{
		//InoGISSInstance->AddToRoot();
		return true;
	}
	
	return false;
}

bool UFL_SS::AddAllInoGISubsystems(UGameInstance* GameInstance)
{
	if (!GameInstance)
	{
		return false;
	}

	TArray<TSubclassOf<UInoGISS>> Subclasses;
	for (TObjectIterator<UClass> It; It; ++It)
	{
		if (It->IsChildOf(UInoGISS::StaticClass()) && !It->HasAnyClassFlags(CLASS_Abstract))
		{
			UE_LOG(InoSSGI, Warning, TEXT("class=%s"), *It->GetName());
			
			Subclasses.Add(*It);
		}
	}
	if (Subclasses.Num() == 0)
	{
		return false;
	}

	for (TSubclassOf<UInoGISS>& Subclass : Subclasses)
	{
		// Create an instance of the subclass and initialize it
		UInoGISS* NewSubsystem = NewObject<UInoGISS>(GameInstance, Subclass);
	}
	return true;
}
