/* Copyright (c) 2021-2023 by Inoland */

#include "FunctionLibraries/FL_Base.h"

#include "HAL/PlatformApplicationMisc.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Engine/World.h"
#include "Engine/GameViewportClient.h"

#include "Misc/ConfigCacheIni.h"


#include "GameFramework/PlayerController.h"
#include "Misc/OutputDeviceNull.h"

#include "InoBase/SS_InoBase.h"

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
			const FString& Input,
			float Duration,
			const FGameplayTag& Type,
			const FGameplayTag& Category,
			bool bScreenPrint,
			bool bOutputPtint,
			bool bInoLog,
			bool bServerLog,
			FLinearColor CustomColor,
			const FString& key,
			bool bShowObjectName,
			bool bShowCategory,
			bool bShowTime
			)
{
	FGameplayTag ActualType = Type.IsValid() ? Type : FGameplayTag::RequestGameplayTag(InoLogTypes::Unkown.GetTag().GetTagName());
	FGameplayTag ActualCategory = Category.IsValid() ? Category : FGameplayTag::RequestGameplayTag(InoLogCategory::Unkown.GetTag().GetTagName());

	//color system
	FLinearColor TempColor;
	
	if (CustomColor == FLinearColor(1.0, 1.0, 1.0))
	{
		if (ActualType == InoLogTypes::Critital)
		{
			TempColor = FLinearColor(1, 0, 0.711481,1);
		}else if (ActualType == InoLogTypes::Error)
		{
			TempColor = FLinearColor(1, 0, 0,1);
		}else if (ActualType == InoLogTypes::Info)
		{
			TempColor = FLinearColor(0, 0, 1,1);
		}else if (ActualType == InoLogTypes::Success)
		{
			TempColor = FLinearColor(0.05399, 1, 0,1);
		}else if (ActualType == InoLogTypes::Unkown)
		{
			TempColor = FLinearColor(0, 1, 0.889834,1);
		}else if (ActualType == InoLogTypes::Warning)
		{
			TempColor = FLinearColor(1, 0.464954, 0,1);
		}
	}else{
		TempColor = CustomColor;
	}
	
	FString Output = "";

	if (bShowTime){
		Output.Append(FDateTime::UtcNow().ToString());
		Output.Append("->");
	}

	if (bShowCategory){
		Output.Append(ActualCategory.ToString());
		Output.Append("->");
	}

	FString TempObjectName= TEXT("Unkown");;
	if (WorldContextObject && bShowObjectName){
		TempObjectName = WorldContextObject->GetName();
		Output.Append(TempObjectName);
		Output.Append("->");
	}
	
	Output.Append(Input);
	
	if (bScreenPrint || bOutputPtint)
	{
		UKismetSystemLibrary::PrintString(WorldContextObject, Output, bScreenPrint, bOutputPtint, TempColor, Duration, (key.IsEmpty()) ?  NAME_None : FName(*key));
	}

	if (bInoLog || bServerLog)
	{
		UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
		if (GameInstance)
		{
			UInoBase* Inobase =	GameInstance->GetSubsystem<UInoBase>();
			if (Inobase)
			{
				Inobase->InoLogCalled(Input, ActualType, ActualCategory,   TempObjectName, bInoLog, bServerLog);
			}
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