/* Copyright (c) 2021-2023 by Inoland */

#include "FunctionLibraries/FL_Base.h"

#include "HAL/PlatformApplicationMisc.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Engine/World.h"
#include "Engine/GameViewportClient.h"

#include "Misc/ConfigCacheIni.h"

#include "UObject/UnrealType.h"

#include "GameFramework/PlayerController.h"
#include "Misc/OutputDeviceNull.h"

DEFINE_LOG_CATEGORY(InoLogCat);

UFL_Base::UFL_Base(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool UFL_Base::IsEditor()
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}

bool UFL_Base::GetEnvironmentVariable(FString Env_Variable, FString& Env_Value)
{
	Env_Value = FPlatformMisc::GetEnvironmentVariable(*Env_Variable);
	return !Env_Value.IsEmpty();
}

void UFL_Base::InoLog(
	const UObject* WorldContextObject,
	const FString& InString,

	const EInoLogType Type,
	const FGameplayTag& Category,

	float Duration,
	bool bPrintToScreen,
	bool bPrintToLog,
	const FName Key,
				
	FLinearColor CustomColor,
				
	bool bAddTime,
	bool bActive
			)
{
	if(!bActive)
	{
		return;
	}
	
	FLinearColor TypeColor;
	if (CustomColor == FLinearColor(1.0, 1.0, 1.0))
	{
		switch (Type)
		{
		case EInoLogType::Display:
			TypeColor = FLinearColor(0.0f, 0.66f, 1.0f);
			break;

		case EInoLogType::Success:
			TypeColor = FLinearColor(0.0f, 1.0f, 0.1);
			break;
			
		case EInoLogType::Warning:
			TypeColor = FLinearColor(1.0f, 1.0f, 0.0f);
			break;

		case EInoLogType::Error:
			TypeColor = FLinearColor(1.0f, 0.1f, 0.0f);
			break;

		case EInoLogType::Fatal:
			TypeColor = FLinearColor(1.0f, 0.0f, 1.0f);
			break;

		default:
			TypeColor = FLinearColor(0.0f, 0.66f, 1.0f);
			break;
		}
	}else{
		TypeColor = CustomColor;
	}
	
	FString OutputToShow = TEXT("");
	
	if (bAddTime){
		OutputToShow.Append(FDateTime::UtcNow().ToString());
		OutputToShow.Append("(>>)");
	}

	if (Category.IsValid()){
		OutputToShow.Append(Category.ToString());
		OutputToShow.Append("(>>)");
	}
	
	if (WorldContextObject){
		OutputToShow.Append(WorldContextObject->GetName());
		OutputToShow.Append("(>>)");
	}

	OutputToShow.Append(InString);
	
	UKismetSystemLibrary::PrintString(WorldContextObject, OutputToShow, bPrintToScreen, bPrintToLog, TypeColor, Duration, Key);
	
	if(bPrintToLog)
	{
		switch (Type)
		{
			case EInoLogType::Display:
				//UE_LOG(InoLogCat, Display, TEXT("%s"), *OutputToShow);
				break;

			case EInoLogType::Success:
				//UE_LOG(InoLogCat, Log, TEXT("%s"), *OutputToShow);
				break;

			case EInoLogType::Warning:
				UE_LOG(InoLogCat, Warning, TEXT("%s"), *OutputToShow);
				break;

			case EInoLogType::Error:
				UE_LOG(InoLogCat, Error, TEXT("%s"), *OutputToShow);
				break;

			case EInoLogType::Fatal:
				UE_LOG(InoLogCat, Fatal, TEXT("%s"), *OutputToShow);
				break;

			default:
				//UE_LOG(InoLogCat, Display, TEXT("%s"), *OutputToShow);
				break;
		}
	}
}


EViewMode UFL_Base::GetCurrentViewMode(const APlayerController *PlayerController)
{
	if (PlayerController)
	{
		UGameViewportClient* GameViewportClient = PlayerController->GetWorld()->GetGameViewport();
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

		bool ignore = GameViewportClient->IgnoreInput();
		EMouseCaptureMode capt = GameViewportClient->GetMouseCaptureMode();
		if (ignore == false && capt == EMouseCaptureMode::CaptureDuringMouseDown)
		{
			return EViewMode::GameAndUI;
		}
		else if (ignore == true && capt == EMouseCaptureMode::NoCapture)
		{
			return EViewMode::UIOnly;
		}
		else
		{
			return EViewMode::GameOnly;
		}
	}
	return EViewMode::None;
}

void UFL_Base::SetClipboard(FString InputString)
{
	FPlatformApplicationMisc::ClipboardCopy(*InputString);
}

void UFL_Base::GetClipboard(FString& OutputString)
{
	FPlatformApplicationMisc::ClipboardPaste(OutputString);
}


FString UFL_Base::GetProjectVersion()
{
	FString AppVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		AppVersion,
		GGameIni
	);
	return AppVersion;
}

bool UFL_Base::CallBlueprintFunctionByName(UObject* TargetObject, const FName& FunctionName)
{
	if (!TargetObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetObject is null"));
		return false;
	}

	UFunction* Function = TargetObject->FindFunction(FunctionName);

	if (Function)
	{
		TargetObject->ProcessEvent(Function, nullptr);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Function %s not found on object %s"), *FunctionName.ToString(), *TargetObject->GetName());
		return false;
	}
}
