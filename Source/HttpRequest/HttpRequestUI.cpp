// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpRequestUI.h"
#include "Kismet/GameplayStatics.h"
#include "NodejsGameModeBase.h"

void UHttpRequestUI::NativeConstruct()
{
	Super::NativeConstruct();

	GameModeRef = Cast<ANodejsGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (SendJsonButton)
	{
		SendJsonButton->OnClicked.AddDynamic(this, &UHttpRequestUI::OnSendJsonClicked);
	}

	if (ConnectWebSocketButton)
	{
		ConnectWebSocketButton->OnClicked.AddDynamic(this, &UHttpRequestUI::OnConnectWebSocketClicked);
	}

	if (SendWebSocketButton)
	{
		SendWebSocketButton->OnClicked.AddDynamic(this, & UHttpRequestUI::OnSendWebSocketClicked);
	}
}

void UHttpRequestUI::OnSendJsonClicked()
{
	if (GameModeRef)
	{
		GameModeRef->MyHTTPPostRequest();
	}
}

void UHttpRequestUI::OnConnectWebSocketClicked()
{
	if (GameModeRef)
	{
		GameModeRef->ConnectWebSocket(ServerURL);
	}
}

void UHttpRequestUI::OnSendWebSocketClicked()
{
	if (GameModeRef)
	{
		GameModeRef->SendJsonViaWebSocket();
	}
}

void UHttpRequestUI::SetURL(const FString& NewURL)
{
	ServerURL = NewURL;
}

void UHttpRequestUI::SetButtonsEnabled(bool bEnabled)
{
	if (SendJsonButton)
	{
		SendJsonButton->SetIsEnabled(bEnabled);
	}

	if (ConnectWebSocketButton)
	{
		ConnectWebSocketButton->SetIsEnabled(bEnabled);
	}

	if (SendWebSocketButton)
	{
		SendWebSocketButton->SetIsEnabled(bEnabled);
	}
}

void UHttpRequestUI::OnWebSocketConnected()
{
	SetButtonsEnabled(true);
}

void UHttpRequestUI::OnWebSocketDisConnected()
{
	SetButtonsEnabled(false);
}
