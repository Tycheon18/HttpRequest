// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "JsonSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class HTTPREQUEST_API UJsonSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JsonData")
	FString SavedJsonData;
	
};
