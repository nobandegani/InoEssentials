/* Copyright (c) 2021-2024 by Inoland */

#pragma once

#include "CoreMinimal.h"
#include "InoSubsystemTypeActions.h"

class FInoSubsystemEditorModule : public IModuleInterface
{
protected:
    TArray<TSharedPtr<FInoSubsystemTypeActionsBase>> ActionTypes;
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
