/* Copyright (c) 2021-2024 by Inoland */

#pragma once

#include "WebBrowser.h"

#include "InoWebBrowser.generated.h"


UCLASS()
class INOESSENTIALS_API UInoWebBrowser : public UWebBrowser
{
	GENERATED_UCLASS_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSimpleEvent, bool, nothing);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTitleChanged, const FText&, Text);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnLoadUrl, const FString&, Method, const FString&, Url, const FString&, Response);
	
	UFUNCTION(BlueprintCallable, Category="Web Browser")
		void Reload();
	
	UFUNCTION(BlueprintCallable, Category="Web Browser")
		void StopLoad();

	UFUNCTION(BlueprintCallable, Category="Web Browser")
		FText GetAddressBarUrlText();

	UFUNCTION(BlueprintCallable, Category="Web Browser")
		bool IsLoaded();

	UFUNCTION(BlueprintCallable, Category="Web Browser")
		bool IsLoading();

	///UFUNCTION(BlueprintCallable, Category="Web Browser")
	//void GetSource(TFunction<void (const FString&)> Callback) const;

	UFUNCTION(BlueprintCallable, Category="Web Browser")
		bool CanGoBack() const;

	UFUNCTION(BlueprintCallable, Category="Web Browser")
		void GoBack();

	UFUNCTION(BlueprintCallable, Category="Web Browser")
		bool CanGoForward() const;

	UFUNCTION(BlueprintCallable, Category="Web Browser")
		void GoForward();

	UPROPERTY(BlueprintAssignable, Category = "Web Browser|Event")
		FSimpleEvent OnLoadCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Web Browser|Event")
		FSimpleEvent OnLoadError;

	UPROPERTY(BlueprintAssignable, Category = "Web Browser|Event")
		FSimpleEvent OnLoadStarted;

	UPROPERTY(BlueprintAssignable, Category = "Web Browser|Event")
		FOnUrlChanged OnTitleChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Web Browser|Event")
		FOnLoadUrl OnLoadUrl;
protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface
	
	void HandleOnLoadCompleted();
	void HandleOnLoadError();
	void HandleOnLoadStarted();

	void HandleOnTitleChanged(const FText& InText);
	
	bool HandleOnLoadUrl(const FString& Method, const FString& Url, FString& Response);
};

