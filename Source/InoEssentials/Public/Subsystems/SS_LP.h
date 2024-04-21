/* Copyright (c) 2021-2023 by Inoland */

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Subsystems/LocalPlayerSubsystem.h"

#include "SS_LP.generated.h"

class ULocalPlayerSubsystem;
struct FWorldContext;

UCLASS(Abstract, Within = LocalPlayer, Blueprintable, BlueprintType, Category="Ino SS LP")
class INOESSENTIALS_API UInoLPSS : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

};

