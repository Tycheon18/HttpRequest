// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpRequestGameModeBase.h"

AHttpRequestGameModeBase::AHttpRequestGameModeBase()
{
	Http = &FHttpModule::Get();
}

void AHttpRequestGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AHttpRequestGameModeBase::SendHTTPGet()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpRequestGameModeBase::OnGetTimeResponse);
	Request->SetURL("");
	Request->SetVerb("GET");
	Request->SetHeader("User-Agent", "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", "application/json");
	Request->ProcessRequest();
}

void AHttpRequestGameModeBase::OnGetTimeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	TSharedPtr<FJsonObject> JsonObject;
	if (Response->GetResponseCode() == 200)
	{
		//const FString ResponseBody = Response->GetContentAsString();

		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{

		}
	}
}
