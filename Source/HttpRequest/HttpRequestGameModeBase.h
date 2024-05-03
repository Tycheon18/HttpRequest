// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Http.h"
#include "HttpRequestGameModeBase.generated.h"

/**
 * 
 */
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

	FHttpModule* Http;
};
