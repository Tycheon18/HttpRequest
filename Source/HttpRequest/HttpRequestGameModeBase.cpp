// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpRequestGameModeBase.h"
#include "Kismet/KismetMathLibrary.h"

AHttpRequestGameModeBase::AHttpRequestGameModeBase():
	City(ECity::EC_Seoul)
{
	Http = &FHttpModule::Get();
}

void AHttpRequestGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SendHTTPGet();
}

void AHttpRequestGameModeBase::SendHTTPGet()
{
	SwitchOnCity();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpRequestGameModeBase::OnGetTimeResponse);
	Request->SetURL(CityURL);
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
		const FString ResponseBody = Response->GetContentAsString();

		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			UKismetMathLibrary::DateTimeFromIsoString(*JsonObject->GetStringField("dateTime"), Time);
		}
	}
}

void AHttpRequestGameModeBase::SwitchOnCity()
{
	CityURL = FString("https://timeapi.io/api/Time/current/zone?timeZone=");

	switch (City)
	{
	case ECity::EC_Seoul:
		CityURL.Append("Asia/Seoul");
		break;
	case ECity::EC_Tokyo:
		CityURL.Append("Asia/Tokyo");
		break;
	case ECity::EC_NewYork:
		CityURL.Append("America/New_York");
		break;
	case ECity::EC_Default:
		break;		
	}
}
