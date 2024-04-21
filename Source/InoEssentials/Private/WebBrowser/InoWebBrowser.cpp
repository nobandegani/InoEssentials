/* Copyright (c) 2021-2022 by Inoland */

#include "WebBrowser/InoWebBrowser.h"
#include "SWebBrowser.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Async/TaskGraphInterfaces.h"
#include "UObject/ConstructorHelpers.h"

#if WITH_EDITOR
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialExpressionMaterialFunctionCall.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "Materials/MaterialExpressionTextureSampleParameter2D.h"
#include "Materials/MaterialFunction.h"
#include "Factories/MaterialFactoryNew.h"
//#include "AssetRegistryModule.h"
#include "PackageHelperFunctions.h"
#endif

#define LOCTEXT_NAMESPACE "InoWebBrowser"


UInoWebBrowser::UInoWebBrowser(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = true;
}

void UInoWebBrowser::Reload()
{
	if ( WebBrowserWidget.IsValid() )
	{
		return WebBrowserWidget->Reload();
	}
}

void UInoWebBrowser::StopLoad()
{
	if ( WebBrowserWidget.IsValid() )
	{
		return WebBrowserWidget->StopLoad();
	}
}

FText UInoWebBrowser::GetAddressBarUrlText()
{
	if ( WebBrowserWidget.IsValid() )
	{
		return WebBrowserWidget->GetAddressBarUrlText();
	}
	return FText();
}

bool UInoWebBrowser::IsLoaded()
{
	if ( WebBrowserWidget.IsValid() )
	{
		return WebBrowserWidget->IsLoaded();
	}
	return false;
}

bool UInoWebBrowser::IsLoading()
{
	if ( WebBrowserWidget.IsValid() )
	{
		return WebBrowserWidget->IsLoading();
	}
	return false;
}

bool UInoWebBrowser::CanGoBack() const
{
	if ( WebBrowserWidget.IsValid() )
	{
		return WebBrowserWidget->CanGoBack();
	}
	return false;
}

void UInoWebBrowser::GoBack()
{
	if ( WebBrowserWidget.IsValid() )
	{
		return WebBrowserWidget->GoBack();
	}
}

bool UInoWebBrowser::CanGoForward() const
{
	if ( WebBrowserWidget.IsValid() )
	{
		return WebBrowserWidget->CanGoForward();
	}
	return false;
}

void UInoWebBrowser::GoForward()
{
	if ( WebBrowserWidget.IsValid() )
	{
		return WebBrowserWidget->GoForward();
	}
}

TSharedRef<SWidget> UInoWebBrowser::RebuildWidget()
{
	if ( IsDesignTime() )
	{
		return SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Web Browser", "Web Browser"))
			];
	}
	else
	{
		WebBrowserWidget = SNew(SWebBrowser)
			.InitialURL(InitialURL)
			.ShowControls(false)
			.SupportsTransparency(bSupportsTransparency)
			.OnLoadCompleted(BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleOnLoadCompleted))
			.OnLoadError(BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleOnLoadError))
			.OnLoadStarted(BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleOnLoadStarted))
			.OnTitleChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnTitleChanged))
			.OnLoadUrl(BIND_UOBJECT_DELEGATE(SWebBrowser::FOnLoadUrl, HandleOnLoadUrl))
		
			.OnUrlChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnUrlChanged))
			.OnBeforePopup(BIND_UOBJECT_DELEGATE(FOnBeforePopupDelegate, HandleOnBeforePopup));

		return WebBrowserWidget.ToSharedRef();
	}
}

void UInoWebBrowser::HandleOnLoadCompleted()
{
	if (OnLoadCompleted.IsBound())
	{
		OnLoadCompleted.Broadcast(true);
	}
}

void UInoWebBrowser::HandleOnLoadError()
{
	if (OnLoadError.IsBound())
	{
		OnLoadError.Broadcast(true);
	}
}

void UInoWebBrowser::HandleOnLoadStarted()
{
	if (OnLoadStarted.IsBound())
	{
		OnLoadStarted.Broadcast(true);
	}
}

void UInoWebBrowser::HandleOnTitleChanged(const FText& InText)
{
	if (OnTitleChanged.IsBound())
	{
		OnTitleChanged.Broadcast(InText);
	}
}

bool UInoWebBrowser::HandleOnLoadUrl(const FString& Method, const FString& Url, FString& Response)
{
	if (OnLoadUrl.IsBound())
	{
		OnLoadUrl.Broadcast(Method, Url, Response);
		return true;
	}
	return false;
}
