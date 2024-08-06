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
