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

DECLARE_LOG_CATEGORY_EXTERN(InoLogCat, All, All);

/**
 * 
 */
UCLASS(Blueprintable, meta = (DisplayName = "Ino FL Base", Keywords = "base"), Category = "Ino|FL|Base")
class UFL_Base : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFL_Base(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Is Editor?", Keywords = "is editor "), Category = "Ino|FL|Base")
		static bool IsEditor();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Environment Variable", Keywords = "Get Environment Variable"), Category = "Ino|FL|Base")
		static bool GetEnvironmentVariable(FString Env_Variable, FString& Env_Value);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Ino Log", Keywords = "Print, String, Log, print string", WorldContext="WorldContextObject", CallableWithoutWorldContext, HidePin = "WorldContextObject", AdvancedDisplay = "5", AutoCreateRefTerm = "Category"), Category = "Ino|FL|Base")
		static void InoLog(
			const UObject* WorldContextObject,
			const FString& InString = FString(TEXT("Hello")),

			const EInoLogType Type = EInoLogType::Display,
			const FGameplayTag& Category = FGameplayTag(),

			float Duration = 2.f,
			bool bPrintToScreen = true,
			bool bPrintToLog = true,
			const FName Key = NAME_None,
			
			FLinearColor CustomColor = FLinearColor(1.0, 1.0, 1.0),
			
			bool bAddTime = false,
			bool bActive = true
			); 
	
	UFUNCTION(BlueprintCallable, Category = "Ino|FL|Base", meta  = (DisplayName = "Get Current ViewMode", Keywords = "get current input mode, input mode, ui mode"))
		static EViewMode GetCurrentViewMode(const APlayerController *PlayerController);

	UFUNCTION(BlueprintCallable, Category = "Ino|FL|Base")
		static void SetClipboard(FString InputString);
	
	UFUNCTION(BlueprintCallable, Category = "Ino|FL|Base")
		static void GetClipboard(FString &OutputString);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Project Version"), Category = "Ino|FL|Base")
		static FString GetProjectVersion();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Call Blueprint Function By Name"), Category = "Ino|FL|Base")
	static bool CallBlueprintFunctionByName(UObject* TargetObject, const FName& FunctionName);
};
