#include "InoDataAsset.h"


UInoDataAsset::UInoDataAsset()
{
	
}

#if WITH_EDITOR
void UInoDataAsset::PostEditChangeProperty(FPropertyChangedEvent& ChangedEvent)
{/*
	if (ChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_STRING_VIEW_CHECKED(ThisClass, Mantling))
	{
		Mantling.PostEditChangeProperty(ChangedEvent);
	}
*/
	Super::PostEditChangeProperty(ChangedEvent);
}
#endif
