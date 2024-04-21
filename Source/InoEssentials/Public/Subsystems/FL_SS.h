// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Subsystems/SS_GI.h"

#include "FL_SS.generated.h"

/**
 * 
 */
UCLASS()
class INOESSENTIALS_API UFL_SS : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	static bool AddInoGISubsystem(UGameInstance* GameInstance, TSubclassOf<UInoGISS> InoGISSSubclass);

	UFUNCTION(BlueprintCallable)
	static bool AddAllInoGISubsystems(UGameInstance* GameInstance);
};
