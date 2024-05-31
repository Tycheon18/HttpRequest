// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpRequestGameModeBase.h"
#include "Kismet/KismetMathLibrary.h"

AHttpRequestGameModeBase::AHttpRequestGameModeBase():
	City(ECity::EC_Seoul),
	SecondCountFloat(1.f)
{
	Http = &FHttpModule::Get();
}

void AHttpRequestGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SendHTTPGet();
	GetWorldTimerManager().SetTimer(SecondCounter, this, &AHttpRequestGameModeBase::SecondCounterCallback, SecondCountFloat);
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
			BreakTime();
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

void AHttpRequestGameModeBase::BreakTime()
{
	int32 Year;
	int32 Month;
	int32 Day;
	int32 MilliSecond;

	UKismetMathLibrary::BreakDateTime(Time, Year, Month, Day, Hour, Minute, Second, MilliSecond);
}

void AHttpRequestGameModeBase::SecondCounterCallback()
{
	Time += FTimespan::FromSeconds(1);
	BreakTime();

	GetWorldTimerManager().SetTimer(SecondCounter, this, &AHttpRequestGameModeBase::SecondCounterCallback, SecondCountFloat);
}

FText AHttpRequestGameModeBase::GetCurrentTime()
{
	FString Hours;
	FString Minutes;
	FString Seconds;

	if (Hour < 10)
	{
		Hours = FString("0").Append(FString::FromInt(Hour));
	}
	else
	{
		Hours = FString::FromInt(Hour);
	}

	if (Minute < 10)
	{
		Minutes = FString("0").Append(FString::FromInt(Minute));
	}
	else
	{
		Minutes = FString::FromInt(Minute);
	}

	if (Second < 10)
	{
		Seconds = FString("0").Append(FString::FromInt(Second));
	}
	else
	{
		Seconds = FString::FromInt(Second);
	}

	FString ReturnString = Hours.Append(FString(": ")).Append(Minutes).Append(FString(": ")).Append(Seconds);

	return FText::FromString(ReturnString);
}

void AHttpRequestGameModeBase::SetCurrentCity(ECity CurrentCity)
{
	City = CurrentCity;
	SendHTTPGet();
}
