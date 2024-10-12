// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Http.h"
#include "NodejsGameModeBase.generated.h"

USTRUCT(BlueprintType)
struct FJsonBaseData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JsonBaseData")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JsonBaseData")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JsonBaseData")
	FString Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JsonBaseData")
	int32 Number;
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

protected:

	void SendHTTPGet();

	void SendHTTPPost();

private:

	FJsonBaseData JsonBaseData;

	FHttpModule* Http;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Allowprivateaccess = true), Category = "HTTP")
	FString ResponseBody;
public:

	void OnGetResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	UFUNCTION(BlueprintCallable , Category = "HTTP")
	void MyHTTPGetRequest();

	UFUNCTION(BlueprintCallable, Category = "HTTP")
	void MyHTTPPostRequest();

	UFUNCTION(BlueprintCallable, Category = "HTTP")
	void SetJsonBaseName(FText NewName);

	UFUNCTION(BlueprintCallable, Category = "HTTP")
	void SetJsonBaseDescription(FText NewDescription);

	UFUNCTION(BlueprintCallable, Category = "HTTP")
	void SetJsonBaseColor(FText NewColor);

	UFUNCTION(BlueprintCallable, Category = "HTTP")
	void SetJsonBaseNumber(FText NewNumber);

};
