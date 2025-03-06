// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Http.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "NodejsGameModeBase.generated.h"

USTRUCT(BlueprintType)
struct FJsonBaseData
{
	GENERATED_BODY()

public:

	FJsonBaseData()
		: Name(TEXT("Dafalut Name"))
		, Description(TEXT("Default Description"))
		, Color(TEXT("White"))
		, Number(0)
		, CreatedAt(FDateTime::UtcNow().ToString())
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JsonBaseData")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JsonBaseData")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JsonBaseData")
	FString Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JsonBaseData")
	int32 Number;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JsonBaseDate")
	FString CreatedAt;

	FString ToJson() const
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		JsonObject->SetStringField(TEXT("name"), Name);
		JsonObject->SetStringField(TEXT("description"), Description);
		JsonObject->SetStringField(TEXT("color"), Color);
		JsonObject->SetNumberField(TEXT("number"), Number);
		JsonObject->SetStringField(TEXT("created_at"), CreatedAt);

		FString JsonString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
		FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

		return JsonString;
	}

	void FromJson(const FString& JsonString)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			Name = JsonObject->GetStringField(TEXT("name"));
			Description = JsonObject->GetStringField(TEXT("description"));
			Color = JsonObject->GetStringField(TEXT("color"));
			Number = JsonObject->GetNumberField(TEXT("number"));
			CreatedAt = JsonObject->GetStringField(TEXT("created_at"));
		}
	}
};
/**
 * 
 */
UCLASS()
class HTTPREQUEST_API ANodejsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ANodejsGameModeBase();

	virtual void BeginPlay() override;

protected:

	TSharedPtr<IWebSocket> WebSocket;
	
	UFUNCTION(BlueprintCallable, Category = "HTTP")
	void SendHTTPGet();

	UFUNCTION(BlueprintCallable, Category = "JSON")
	void SendHTTPJsonPost(const FString& URL , const FJsonBaseData& JsonData);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WebSocket")
	bool bIsWebSocketConnected = false;

private:

	FJsonBaseData JsonBaseData;

	FHttpModule* Http;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Allowprivateaccess = true), Category = "HTTP")
	FString ResponseBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Allowprivateaccess = true), Category = "HTTP")
	FString LastHttpResponse;

public:

	void OnGetResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void OnPostResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	UFUNCTION(BlueprintCallable , Category = "HTTP")
	void MyHTTPGetRequest();

	UFUNCTION(BlueprintCallable, Category = "HTTP")
	void MyHTTPPostRequest();

	UFUNCTION(BlueprintCallable, Category = "HTTP")
	void UpdateHttpResponse(const FString& ResponseText);

	//UFUNCTION(BlueprintCallable, Category = "HTTP")
	void OnHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	UFUNCTION(BlueprintCallable, Category = "JSON")
	void SetJsonBaseName(FText NewName);

	UFUNCTION(BlueprintCallable, Category = "JSON")
	void SetJsonBaseDescription(FText NewDescription);

	UFUNCTION(BlueprintCallable, Category = "JSON")
	void SetJsonBaseColor(FText NewColor);

	UFUNCTION(BlueprintCallable, Category = "JSON")
	void SetJsonBaseNumber(FText NewNumber);

	UFUNCTION(BlueprintCallable, Category = "JSON")
	void SetJsonBaseCreatedAt(FText NewCreatedAt);

	UFUNCTION(BlueprintCallable, Category = "WebSocket")
	void ConnectWebSocket(const FString& URL);

	UFUNCTION(BlueprintCallable, Category = "WebSocket")
	void SendJsonViaWebSocket();

	UFUNCTION(BlueprintImplementableEvent, Category = "WebSocket")
	void UpdateWebSocketUI(bool bConnected);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void SaveJsonData();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void LoadJsonData();

protected:


};
