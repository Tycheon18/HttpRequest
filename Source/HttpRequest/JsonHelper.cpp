// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonHelper.h"
#include "NodejsGameModeBase.h"

// Sets default values
AJsonHelper::AJsonHelper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJsonHelper::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJsonHelper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString AJsonHelper::StructToJson(const FJsonBaseData& Data)
{
	return Data.ToJson();
}

FJsonBaseData AJsonHelper::JsonToStruct(const FString& JsonString)
{
	FJsonBaseData Data;
	Data.FromJson(JsonString);

	return Data;
}

