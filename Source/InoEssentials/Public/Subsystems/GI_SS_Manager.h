/* Copyright (c) 2021-2023 by Inoland */

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Subsystems/SS_GI.h"

#include "GI_SS_Manager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Category="Ino|GI|SS|Manager")
class INOESSENTIALS_API UGI_SS_Manager : public UGameInstance
{
	GENERATED_BODY()

	/** virtual function to allow custom GameInstances an opportunity to set up what it needs */
	virtual void Init();

	FObjectSubsystemCollection<UGameInstanceSubsystem> SubsystemCollection;
	
public:
	UFUNCTION(BlueprintCallable, Category="Ino|GI|SS|Manager")
	void CreateSubsystem(TSubclassOf<UInoGISS> InoSubsystemClass);

	UFUNCTION(BlueprintCallable, Category="Ino|GI|SS|Manager")
	void RemoveSubsystem(TSubclassOf<UInoGISS> InoSubsystemClass);
};