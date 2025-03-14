// All right reserverd Inoland

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "FL_API.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnOpenAIResponse, bool, bSuccess, int, ResponseCode, FString, ResponseString);

/**
 * 
 */
UCLASS()
class INOESSENTIALS_API UFL_OpenAI : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Ino|FL|API|OpenAI", meta = (DisplayName = "OpenAI Transcribe Audio From File"))
	static void OpenAITranscribeAudioFromFile(
		const FString& URL,
		const FString& APIKey,
		const FString& ModelType,
		const FString& Language,
		const FString& Prompt,
		const FString& ResponseFormat,
		const FString& ContentType,
		const FString& FilePath,
		const FOnOpenAIResponse& OnResponse
	);

	UFUNCTION(BlueprintCallable, Category = "Ino|FL|API|ElevenLabs", meta = (DisplayName = "ElevenLabs Transcribe Audio From File"))
	static void ElevenLabsTranscribeAudioFromFile(
		const FString& URL,
		const FString& APIKey,
		const FString& ModelType,
		const FString& Language,
		const FString& Prompt,
		const FString& ResponseFormat,
		const FString& ContentType,
		const FString& FilePath,
		const FOnOpenAIResponse& OnResponse
	);
};
