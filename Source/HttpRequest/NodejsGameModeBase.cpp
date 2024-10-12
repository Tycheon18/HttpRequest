// Fill out your copyright notice in the Description page of Project Settings.


#include "NodejsGameModeBase.h"

ANodejsGameModeBase::ANodejsGameModeBase() : ResponseBody("Nothing Yet")
{
	Http = &FHttpModule::Get();
}

void ANodejsGameModeBase::SendHTTPGet()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ANodejsGameModeBase::OnGetResponse);
	Request->SetURL("http://localhost:3000");
	Request->SetVerb("GET");
	Request->SetHeader("User-Agent", "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", "application/json");
	Request->ProcessRequest();
}

void ANodejsGameModeBase::SendHTTPPost()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

	FString RequestBody;

	Request->OnProcessRequestComplete().BindUObject(this, &ANodejsGameModeBase::OnGetResponse);
	Request->SetURL("http://localhost:3000");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();



}

void ANodejsGameModeBase::OnGetResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			ResponseBody = Response->GetContentAsString();
		}
		else
		{
			if (Response.IsValid())
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Something went wrong with the request. Status Code : %d"), Response->GetResponseCode()));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Response is invalid. Server is down or not responding"));
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Failed to Connect to the Server"));
	}
}

void ANodejsGameModeBase::MyHTTPGetRequest()
{
	SendHTTPGet();
}

void ANodejsGameModeBase::MyHTTPPostRequest()
{
}

void ANodejsGameModeBase::SetJsonBaseName(FText NewName)
{
	JsonBaseData.Name = NewName.ToString();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("JsonBaseData Name is : %s"), *(JsonBaseData.Name)));
}

void ANodejsGameModeBase::SetJsonBaseDescription(FText NewDescription)
{
	JsonBaseData.Description = NewDescription.ToString();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("JsonBaseData Description is : %s"), *(JsonBaseData.Description)));
}

void ANodejsGameModeBase::SetJsonBaseColor(FText NewColor)
{
	JsonBaseData.Color = NewColor.ToString();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("JsonBaseData Color is : %s"), *(JsonBaseData.Color)));
}

void ANodejsGameModeBase::SetJsonBaseNumber(FText NewNumber)
{
	JsonBaseData.Number = FCString::Atoi(*(NewNumber.ToString()));

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("JsonBaseData Number is : %d"), JsonBaseData.Number));
}
