// Fill out your copyright notice in the Description page of Project Settings.


#include "NodejsGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "JsonSaveGame.h"

ANodejsGameModeBase::ANodejsGameModeBase() : ResponseBody("Nothing Yet")
{
	Http = &FHttpModule::Get();
	JsonBaseData = FJsonBaseData();
}

void ANodejsGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	LoadJsonData();
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
				UE_LOG(LogTemp, Log, TEXT("Post Success! Response : %s"), *Response->GetContentAsString());
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

void ANodejsGameModeBase::UpdateHttpResponse(const FString& ResponseText)
{
	LastHttpResponse = ResponseText;
	UE_LOG(LogTemp, Log, TEXT("서버 응답 : %s"), *LastHttpResponse);
}

void ANodejsGameModeBase::OnHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully && Response.IsValid())
	{
		UpdateHttpResponse(Response->GetContentAsString());
	}
	else
	{
		UpdateHttpResponse(TEXT("Server Response Failed"));
	}
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

		WebSocket->OnConnected().AddLambda([this]()
			{
				bIsWebSocketConnected = true;
				UE_LOG(LogTemp, Log, TEXT("WebSocket 연결 성공!"));
				UpdateWebSocketUI(true);
			});

		WebSocket->OnConnectionError().AddLambda([this](const FString& Error)
			{
				bIsWebSocketConnected = false;
				UE_LOG(LogTemp, Log, TEXT("WebSocket 연결 실패 : %s"), *Error);
				UpdateWebSocketUI(false);
			});

		WebSocket->OnClosed().AddLambda([this](int32 StatusCode, const FString& Reason, bool bWasClear)
			{
				bIsWebSocketConnected = false;
				UE_LOG(LogTemp, Log, TEXT("WebSocket 연결 해제 : %s"), *Reason);
				UpdateWebSocketUI(false);
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
	
}

void ANodejsGameModeBase::SaveJsonData()
{
	UJsonSaveGame* SaveGameInstance = NewObject<UJsonSaveGame>();

	if (SaveGameInstance)
	{
		SaveGameInstance->SavedJsonData = JsonBaseData.ToJson();

		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("JsonSaveSlot"), 0);
		UE_LOG(LogTemp, Log, TEXT("JSON 데이터 저장"));
	}
}

void ANodejsGameModeBase::LoadJsonData()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("JsonSaveSlot"), 0))
	{
		UJsonSaveGame* LoadGameInstance = StaticCast<UJsonSaveGame*>(UGameplayStatics::LoadGameFromSlot(TEXT("JsonSaveSlot"), 0));

		if (LoadGameInstance)
		{

			JsonBaseData.FromJson(LoadGameInstance->SavedJsonData);
			UE_LOG(LogTemp, Log, TEXT("JSON 데이터 로딩"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("저장된 JSON 데이터가 없음."));
	}
}

