// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Http.h"
#include "HttpRequestGameModeBase.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ECity :uint8
{
	EC_Seoul	UMETA(DisplayName = "Seoul"),
	EC_Tokyo	UMETA(DisplayName = "Tokyo"),
	EC_NewYork	UMETA(DisplayName = "NewYork"),
	EC_Default	UMETA(DisplayName = "Default")
};

UCLASS()
class HTTPREQUEST_API AHttpRequestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AHttpRequestGameModeBase();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void SendHTTPGet();

	void OnGetTimeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void SwitchOnCity();

private:

	FHttpModule* Http;

	ECity City;

	FString CityURL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Time, meta = (AllowPrivateAccess = "true"))
	FDateTime Time;
};
