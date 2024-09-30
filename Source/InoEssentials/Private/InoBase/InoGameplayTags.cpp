#include "InoBase/InoGameplayTags.h"

namespace InoLogCategory
{
	UE_DEFINE_GAMEPLAY_TAG(Unkown, FName{TEXTVIEW("Ino.Log.Unkown")})

	UE_DEFINE_GAMEPLAY_TAG(Pawn, FName{TEXTVIEW("Ino.Log.Pawn")})
	UE_DEFINE_GAMEPLAY_TAG(Character, FName{TEXTVIEW("Ino.Log.Character")})
	UE_DEFINE_GAMEPLAY_TAG(GameMode, FName{TEXTVIEW("Ino.Log.GameMode")})
	UE_DEFINE_GAMEPLAY_TAG(PlayerController, FName{TEXTVIEW("Ino.Log.PlayerController")})
	UE_DEFINE_GAMEPLAY_TAG(PlayerState, FName{TEXTVIEW("Ino.Log.PlayerState")})
	UE_DEFINE_GAMEPLAY_TAG(GameState, FName{TEXTVIEW("Ino.Log.GameState")})

	
	UE_DEFINE_GAMEPLAY_TAG(InoEssentials, FName{TEXTVIEW("Ino.Log.InoEssentials")})
	UE_DEFINE_GAMEPLAY_TAG(InoOss, FName{TEXTVIEW("Ino.Log.InoOss")})
	UE_DEFINE_GAMEPLAY_TAG(InoUI, FName{TEXTVIEW("Ino.Log.InoUI")})
	UE_DEFINE_GAMEPLAY_TAG(InoModal, FName{TEXTVIEW("Ino.Log.InoModal")})
	UE_DEFINE_GAMEPLAY_TAG(InoNotif, FName{TEXTVIEW("Ino.Log.InoNotif")})
	UE_DEFINE_GAMEPLAY_TAG(InoLoading, FName{TEXTVIEW("Ino.Log.InoLoading")})
	UE_DEFINE_GAMEPLAY_TAG(InoBridge, FName{TEXTVIEW("Ino.Log.InoBridge")})
	UE_DEFINE_GAMEPLAY_TAG(InoInteraction, FName{TEXTVIEW("Ino.Log.InoInteraction")})
}