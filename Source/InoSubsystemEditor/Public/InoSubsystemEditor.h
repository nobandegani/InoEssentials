// Copyright YTSS 2023. All Rights Reserved.

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
