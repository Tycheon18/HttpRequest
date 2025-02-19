// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JsonHelper.generated.h"

UCLASS()
class HTTPREQUEST_API AJsonHelper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJsonHelper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "JsonHelper")
	static FString StructToJson(const struct FJsonBaseData& Data);

	UFUNCTION(BlueprintCallable, Category = "JsonHelper")
	static FJsonBaseData JsonToStruct(const FString& JsonString);

};
