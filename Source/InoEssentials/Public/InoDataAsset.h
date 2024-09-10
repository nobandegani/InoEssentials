#pragma once

#include "InoDataAsset.generated.h"

UCLASS(Blueprintable, BlueprintType)
class INOESSENTIALS_API UInoDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UInoDataAsset();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& ChangedEvent) override;
#endif
};
