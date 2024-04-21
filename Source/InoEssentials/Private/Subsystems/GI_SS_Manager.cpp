/* Copyright (c) 2021-2023 by Inoland */

#include "Subsystems/GI_SS_Manager.h"

void UGI_SS_Manager::Init()
{
	Super::Init();
}

void UGI_SS_Manager::CreateSubsystem(TSubclassOf<UInoGISS> InoSubsystemClass)
{
	SubsystemCollection.ActivateExternalSubsystem(InoSubsystemClass);
}

void UGI_SS_Manager::RemoveSubsystem(TSubclassOf<UInoGISS> InoSubsystemClass)
{
	SubsystemCollection.DeactivateExternalSubsystem(InoSubsystemClass);
}