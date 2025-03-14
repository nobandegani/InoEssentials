// All right reserverd Inoland
#include "FunctionLibraries/FL_API.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

void UFL_OpenAI::OpenAITranscribeAudioFromFile(
	const FString& URL,
	const FString& APIKey,
	const FString& ModelType,
	const FString& Language,
	const FString& Prompt,
	const FString& ResponseFormat,
	const FString& ContentType,
	const FString& FilePath,
	const FOnOpenAIResponse& OnResponse)
{
	// Check for valid inputs
	if (URL.IsEmpty() || APIKey.IsEmpty() || FilePath.IsEmpty() || ModelType.IsEmpty())
	{
		OnResponse.ExecuteIfBound(false, -1, "Invalid input parameters.");
		return;
	}

	// Load file data into a byte array
	TArray<uint8> FileData;
	if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
	{
		OnResponse.ExecuteIfBound(false, -1, "Failed to load file.");
		return;
	}

	// Define a boundary string (this can be any string not found in the data)
	FString Boundary = "WebKitFormBoundary7MA4YWxkTrZu0gW";
	FString ContentTypeMain = "multipart/form-data; boundary=" + Boundary;

	// Build the multipart form-data payload:
	FString FileHeader = FString::Printf(TEXT("--%s\r\nContent-Disposition: form-data; name=\"file\"; filename=\"%s\"\r\nContent-Type: %s\r\n\r\n"),
		*Boundary, *FPaths::GetCleanFilename(FilePath), *ContentType);

	FString ModelPart = FString::Printf(TEXT("\r\n--%s\r\nContent-Disposition: form-data; name=\"model\"\r\n\r\n%s\r\n"),
		*Boundary, *ModelType);

	FString LanguagePart = FString::Printf(TEXT("--%s\r\nContent-Disposition: form-data; name=\"language\"\r\n\r\n%s\r\n"),
		*Boundary, *Language);

	FString PromptPart = FString::Printf(TEXT("--%s\r\nContent-Disposition: form-data; name=\"prompt\"\r\n\r\n%s\r\n"),
		*Boundary, *Prompt);

	FString ResponseFormatPart = FString::Printf(TEXT("--%s\r\nContent-Disposition: form-data; name=\"response_format\"\r\n\r\n%s\r\n"),
		*Boundary, *ResponseFormat);

	FString BoundryEndPart = FString::Printf(TEXT("--%s--\r\n"),
		*Boundary);

	// Combine parts into one payload
	TArray<uint8> Payload;
	{
		// Convert FileHeader to UTF-8 and append
		FTCHARToUTF8 FileHeaderUTF8(*FileHeader);
		Payload.Append(reinterpret_cast<const uint8*>(FileHeaderUTF8.Get()), FileHeaderUTF8.Length());

		// Append the binary file data
		Payload.Append(FileData);

		// Convert ModelPart to UTF-8 and append
		FTCHARToUTF8 ModelPartUTF8(*ModelPart);
		Payload.Append(reinterpret_cast<const uint8*>(ModelPartUTF8.Get()), ModelPartUTF8.Length());

		// Convert LanguagePart to UTF-8 and append
		if (!Language.IsEmpty()){
			FTCHARToUTF8 LanguagePartUTF8(*LanguagePart);
			Payload.Append(reinterpret_cast<const uint8*>(LanguagePartUTF8.Get()), LanguagePartUTF8.Length());
		}

		// Convert PromptPart to UTF-8 and append
		if (!Prompt.IsEmpty()){
			FTCHARToUTF8 PromptPartUTF8(*PromptPart);
			Payload.Append(reinterpret_cast<const uint8*>(PromptPartUTF8.Get()), PromptPartUTF8.Length());
		}

		// Convert ResponseFormatPart to UTF-8 and append
		if (!ResponseFormat.IsEmpty()){
			FTCHARToUTF8 ResponseFormatPartUTF8(*ResponseFormatPart);
			Payload.Append(reinterpret_cast<const uint8*>(ResponseFormatPartUTF8.Get()), ResponseFormatPartUTF8.Length());
		}

		FTCHARToUTF8 BoundryEndPartUTF8(*BoundryEndPart);
		Payload.Append(reinterpret_cast<const uint8*>(BoundryEndPartUTF8.Get()), BoundryEndPartUTF8.Length());
	}

	// Create the HTTP request
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(URL);
	HttpRequest->SetVerb("POST");
	HttpRequest->SetHeader("Authorization", "Bearer " + APIKey);
	HttpRequest->SetHeader("Content-Type", ContentTypeMain);
	HttpRequest->SetContent(Payload);

	// Bind the response handler
	HttpRequest->OnProcessRequestComplete().BindLambda([OnResponse](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		int32 ResponseCode = Response.IsValid() ? Response->GetResponseCode() : -1;
		FString ResponseString = Response.IsValid() ? Response->GetContentAsString() : "No response";
		OnResponse.ExecuteIfBound(bWasSuccessful, ResponseCode, ResponseString);
	});

	// Send the request
	HttpRequest->ProcessRequest();
}

void UFL_OpenAI::ElevenLabsTranscribeAudioFromFile(
	const FString& URL,
	const FString& APIKey,
	const FString& ModelType,
	const FString& Language,
	const FString& Prompt,
	const FString& ResponseFormat,
	const FString& ContentType,
	const FString& FilePath,
	const FOnOpenAIResponse& OnResponse)
{
	// Check for valid inputs
	if (URL.IsEmpty() || APIKey.IsEmpty() || FilePath.IsEmpty() || ModelType.IsEmpty())
	{
		OnResponse.ExecuteIfBound(false, -1, "Invalid input parameters.");
		return;
	}

	// Load file data into a byte array
	TArray<uint8> FileData;
	if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
	{
		OnResponse.ExecuteIfBound(false, -1, "Failed to load file.");
		return;
	}

	// Define a boundary string (this can be any string not found in the data)
	FString Boundary = "WebKitFormBoundary7MA4YWxkTrZu0gW";
	FString ContentTypeMain = "multipart/form-data; boundary=" + Boundary;

	// Build the multipart form-data payload:
	FString ModelPart = FString::Printf(TEXT("--%s\r\nContent-Disposition: form-data; name=\"model_id\"\r\n\r\n%s\r\n"),
		*Boundary, *ModelType);
	
	FString FileHeader = FString::Printf(TEXT("--%s\r\nContent-Disposition: form-data; name=\"file\"; filename=\"%s\"\r\nContent-Type: %s\r\n\r\n"),
		*Boundary, *FPaths::GetCleanFilename(FilePath), *ContentType);

	FString LanguagePart = FString::Printf(TEXT("\r\n--%s\r\nContent-Disposition: form-data; name=\"language_code\"\r\n\r\n%s\r\n"),
		*Boundary, *Language);

	FString PromptPart = FString::Printf(TEXT("--%s\r\nContent-Disposition: form-data; name=\"prompt\"\r\n\r\n%s\r\n"),
		*Boundary, *Prompt);

	FString ResponseFormatPart = FString::Printf(TEXT("--%s\r\nContent-Disposition: form-data; name=\"response_format\"\r\n\r\n%s\r\n"),
		*Boundary, *ResponseFormat);

	FString BoundryEndPart = FString::Printf(TEXT("\r\n--%s--\r\n"),
		*Boundary);

	// Combine parts into one payload
	TArray<uint8> Payload;
	{
		// Convert ModelPart to UTF-8 and append
		FTCHARToUTF8 ModelPartUTF8(*ModelPart);
		Payload.Append(reinterpret_cast<const uint8*>(ModelPartUTF8.Get()), ModelPartUTF8.Length());

		
		// Convert FileHeader to UTF-8 and append
		FTCHARToUTF8 FileHeaderUTF8(*FileHeader);
		Payload.Append(reinterpret_cast<const uint8*>(FileHeaderUTF8.Get()), FileHeaderUTF8.Length());

		// Append the binary file data
		Payload.Append(FileData);
		
		// Convert LanguagePart to UTF-8 and append
		if (!Language.IsEmpty()){
			FTCHARToUTF8 LanguagePartUTF8(*LanguagePart);
			Payload.Append(reinterpret_cast<const uint8*>(LanguagePartUTF8.Get()), LanguagePartUTF8.Length());
		}

		// Convert PromptPart to UTF-8 and append
		if (!Prompt.IsEmpty()){
			FTCHARToUTF8 PromptPartUTF8(*PromptPart);
			Payload.Append(reinterpret_cast<const uint8*>(PromptPartUTF8.Get()), PromptPartUTF8.Length());
		}

		// Convert ResponseFormatPart to UTF-8 and append
		if (!ResponseFormat.IsEmpty()){
			FTCHARToUTF8 ResponseFormatPartUTF8(*ResponseFormatPart);
			Payload.Append(reinterpret_cast<const uint8*>(ResponseFormatPartUTF8.Get()), ResponseFormatPartUTF8.Length());
		}

		FTCHARToUTF8 BoundryEndPartUTF8(*BoundryEndPart);
		Payload.Append(reinterpret_cast<const uint8*>(BoundryEndPartUTF8.Get()), BoundryEndPartUTF8.Length());
	}

	// Create the HTTP request
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(URL);
	HttpRequest->SetVerb("POST");
	HttpRequest->SetHeader("xi-api-key", APIKey);
	HttpRequest->SetHeader("Content-Type", ContentTypeMain);
	HttpRequest->SetContent(Payload);

	// Bind the response handler
	HttpRequest->OnProcessRequestComplete().BindLambda([OnResponse](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		int32 ResponseCode = Response.IsValid() ? Response->GetResponseCode() : -1;
		FString ResponseString = Response.IsValid() ? Response->GetContentAsString() : "No response";
		OnResponse.ExecuteIfBound(bWasSuccessful, ResponseCode, ResponseString);
	});

	// Send the request
	HttpRequest->ProcessRequest();
}
