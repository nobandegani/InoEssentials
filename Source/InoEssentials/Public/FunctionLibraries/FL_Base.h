/* Copyright (c) 2021-2023 by Inoland */

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Library/FL_Base_StructLibrary.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"

#include "InoBase/InoGameplayTags.h"

#include "FL_Base.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (DisplayName = "Ino Base FL", Keywords = "base"), Category = "Ino Base FL")
class UFL_Base : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFL_Base(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Is Editor?", Keywords = "is editor "), Category = "Ino Base FL")
		static bool IsEditor();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Environment Variable", Keywords = "Get Environment Variable"), Category = "Ino Base FL")
		static bool GetEnvironmentVariable(FString Env_Variable, FString& Env_Value);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Ino Log", Keywords = "Print, String, Log", WorldContext="WorldContextObject", HidePin = "WorldContextObject", CallableWithoutWorldContext, AdvancedDisplay = "3", GameplayTagFilter = "Ino.Log.", AutoCreateRefTerm = "Type, Category"), Category = "Ino Base FL")
		static void InoLog(
			const UObject* WorldContextObject,
			const FString& InString = FString(TEXT("Hello")),
			float Duration = 5.f,
			const FGameplayTag& Type = FGameplayTag(),
			const FGameplayTag& Category = FGameplayTag(),
			bool bScreenPrint = true,
			bool bOutputPtint = true,
			bool bInoLog = false,
			bool bServerLog = false,
			FLinearColor CustomColor = FLinearColor(1.0, 1.0, 1.0),
			const FString& key = FString(TEXT("None")),
			bool bShowObjectName = true,
			bool bShowCategory = false,
			bool bShowTime = false
			); 
	
	UFUNCTION(BlueprintCallable, Category = "Ino Base FL", meta  = (DisplayName = "Get Current ViewMode", Keywords = "get current input mode, input mode, ui mode"))
		static EViewMode GetCurrentViewMode(const APlayerController *PlayerController);

	UFUNCTION(BlueprintCallable, Category = "Ino Base FL")
		static void SetClipboard(FString InputString);
	
	UFUNCTION(BlueprintCallable, Category = "Ino Base FL")
		static void GetClipboard(FString &OutputString);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Project Version"), Category = "Ino Base FL")
		static FString GetProjectVersion();
};
