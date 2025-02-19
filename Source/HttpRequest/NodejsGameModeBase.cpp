// Fill out your copyright notice in the Description page of Project Settings.


#include "NodejsGameModeBase.h"

ANodejsGameModeBase::ANodejsGameModeBase() : ResponseBody("Nothing Yet")
{
	Http = &FHttpModule::Get();
	JsonBaseData = FJsonBaseData();
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

void ANodejsGameModeBase::SendHTTPJsonPost(const FString& URL, const FJsonBaseData& JsonData)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	//Request->OnProcessRequestComplete().BindUObject(this, &ANodejsGameModeBase::OnPostResponse);

	Request->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (bWasSuccessful && Response.IsValid())
			{
				UE_LOG(LogTemp, Log, TEXT("Post Success! Response : %s"), *Response);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Post Failed!"));
			}

		});

	Request->SetURL(URL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader("Content-Type", "application/json");

	JsonBaseData.CreatedAt = FDateTime::UtcNow().ToString();
	FString JsonPayload = JsonBaseData.ToJson();

	Request->SetContentAsString(JsonPayload);
	Request->ProcessRequest();

}

void ANodejsGameModeBase::OnGetResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		ResponseBody = Response->GetContentAsString();

		JsonBaseData.FromJson(ResponseBody);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Updated JsonBaseData : Name = %s, Description = %s, Color = %s, Number = %d, CreatedAt = %s"), *JsonBaseData.Name, *JsonBaseData.Description, *JsonBaseData.Color, JsonBaseData.Number, *JsonBaseData.CreatedAt));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Failed to Connect to the Server"));
	}
}

void ANodejsGameModeBase::OnPostResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Post Request Successful!"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Post Request Falied.."));
	}
}

void ANodejsGameModeBase::MyHTTPGetRequest()
{
	SendHTTPGet();
}

void ANodejsGameModeBase::MyHTTPPostRequest()
{
	FString URL = TEXT("http://localhost:3000/api/data");

	FJsonBaseData MyData;
	MyData.CreatedAt = FDateTime::UtcNow().ToString();

	SendHTTPJsonPost(URL, MyData);
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

void ANodejsGameModeBase::SetJsonBaseCreatedAt(FText NewCreatedAt)
{
	JsonBaseData.CreatedAt = NewCreatedAt.ToString();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("JsonBaseData CreatedAt is : %s"), *JsonBaseData.CreatedAt));
}

void ANodejsGameModeBase::ConnectWebSocket(const FString& URL)
{
	if (!WebSocket.IsValid())
	{
		FWebSocketsModule& WebSocketModule = FModuleManager::LoadModuleChecked<FWebSocketsModule>("WebSockets");
		WebSocket = WebSocketModule.CreateWebSocket(URL);

		WebSocket->OnConnected().AddLambda([]()
			{
				UE_LOG(LogTemp, Log, TEXT("WebSocket 연결 성공!"));
			});

		WebSocket->OnMessage().AddLambda([](const FString& Message)
			{
				UE_LOG(LogTemp, Log, TEXT("서버로부터 메시지 수신: %s"), *Message);
			});

		WebSocket->OnConnectionError().AddLambda([](const FString& Error)
			{
				UE_LOG(LogTemp, Error, TEXT("WebSocket 연결 실패: %s"), *Error);
			});

		WebSocket->Connect();
	}
}

void ANodejsGameModeBase::SendJsonViaWebSocket()
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		FString JsonString = JsonBaseData.ToJson();
		WebSocket->Send(JsonString);
		UE_LOG(LogTemp, Log, TEXT("WebSocket으로 JSON 전송: %s"), *JsonString);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WebSocket이 연결되지 않음! 먼저 ConnectWebSocket() 호출 필요"));
	}
	
}
