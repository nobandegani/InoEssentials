/* Copyright (c) 2021-2024 by Inoland */

#pragma once

#include "CoreMinimal.h"

#include "Ino_GameInstanceSubsystem.h"
#include "Ino_LocalPlayerSubsystem.h"
#include "Ino_GameInstanceSubsystemTickable.h"
#include "Ino_LocalPlayerSubsystemTickable.h"
#include "Ino_WorldSubsystemTickable.h"
#include "Ino_WorldSubsystem.h"

#include "Kismet2/KismetEditorUtilities.h"
#include "InoSubsystemFactory.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class INOSUBSYSTEMEDITOR_API UInoSubsystemFactory: public UFactory
{
	GENERATED_BODY()
public:
	UInoSubsystemFactory()
	{
		bCreateNew=true;
		bEditAfterNew=true;
	}
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
		UObject* Context, FFeedbackContext* Warn, FName CallingContext) override
	{
		// return NewObject<USubsystem>(InParent,InClass,InName,Flags);
		return FKismetEditorUtilities::CreateBlueprint(InClass, InParent, InName, BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass(), CallingContext);
	}

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
		UObject* Context, FFeedbackContext* Warn) override
	{
		return FactoryCreateNew(InClass,InParent,InName,Flags,Context,Warn,NAME_None);
	}
};


UCLASS(HideCategories=Object)
class INOSUBSYSTEMEDITOR_API UInoSubsystemGameInatanceFactory : public UInoSubsystemFactory
{
	GENERATED_BODY()

public:
	UInoSubsystemGameInatanceFactory()
	{
		SupportedClass=UIno_GameInstanceSubsystem::StaticClass();
	}
};

UCLASS(HideCategories=Object)
class INOSUBSYSTEMEDITOR_API UInoSubsystemWorldFactory : public UInoSubsystemFactory
{
	GENERATED_BODY()

public:
	UInoSubsystemWorldFactory()
	{
		SupportedClass=UIno_WorldSubsystem::StaticClass();
	}
};

UCLASS(HideCategories=Object)
class INOSUBSYSTEMEDITOR_API UInoSubsystemLocalPlayerFactory : public UInoSubsystemFactory
{
	GENERATED_BODY()

public:
	UInoSubsystemLocalPlayerFactory()
	{
		SupportedClass=UIno_LocalPlayerSubsystem::StaticClass();
	}
};



UCLASS(HideCategories=Object)
class INOSUBSYSTEMEDITOR_API UInoSubsystemGameInatanceTickableFactory : public UInoSubsystemFactory
{
	GENERATED_BODY()

public:
	UInoSubsystemGameInatanceTickableFactory()
	{
		SupportedClass=UIno_GameInstanceSubsystemTickable::StaticClass();
	}
};

UCLASS(HideCategories=Object)
class INOSUBSYSTEMEDITOR_API UInoSubsystemWorldTickableFactory : public UInoSubsystemFactory
{
	GENERATED_BODY()

public:
	UInoSubsystemWorldTickableFactory()
	{
		SupportedClass=UIno_WorldSubsystemTickable::StaticClass();
	}
};

UCLASS(HideCategories=Object)
class INOSUBSYSTEMEDITOR_API UInoSubsystemLocalPlayerTickableFactory : public UInoSubsystemFactory
{
	GENERATED_BODY()

public:
	UInoSubsystemLocalPlayerTickableFactory()
	{
		SupportedClass=UIno_LocalPlayerSubsystemTickable::StaticClass();
	}
};