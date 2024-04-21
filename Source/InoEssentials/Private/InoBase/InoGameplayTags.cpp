#include "InoBase/InoGameplayTags.h"

namespace InoLogTypes
{
	UE_DEFINE_GAMEPLAY_TAG(Unkown, FName{TEXTVIEW("Ino.Log.Type.Unkown")})
	UE_DEFINE_GAMEPLAY_TAG(Info, FName{TEXTVIEW("Ino.Log.Type.Info")})
	UE_DEFINE_GAMEPLAY_TAG(Success, FName{TEXTVIEW("Ino.Log.Type.Success")})
	UE_DEFINE_GAMEPLAY_TAG(Warning, FName{TEXTVIEW("Ino.Log.Type.Warning")})
	UE_DEFINE_GAMEPLAY_TAG(Error, FName{TEXTVIEW("Ino.Log.Type.Error")})
	UE_DEFINE_GAMEPLAY_TAG(Critital, FName{TEXTVIEW("Ino.Log.Type.Critital")})
}

namespace InoLogCategory
{
	UE_DEFINE_GAMEPLAY_TAG(Unkown, FName{TEXTVIEW("Ino.Log.Category.Unkown")})
	UE_DEFINE_GAMEPLAY_TAG(InoEssentials, FName{TEXTVIEW("Ino.Log.Category.InoEssentials")})
	UE_DEFINE_GAMEPLAY_TAG(InoOss, FName{TEXTVIEW("Ino.Log.Category.InoOss")})
}