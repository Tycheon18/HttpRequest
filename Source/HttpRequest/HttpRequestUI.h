// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "HttpRequestUI.generated.h"

/**
 * 
 */
UCLASS()
class HTTPREQUEST_API UHttpRequestUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category="Network")
	void OnSendJsonClicked();

	UFUNCTION(BlueprintCallable, Category = "Network")
	void OnConnectWebSocketClicked();

	UFUNCTION(BlueprintCallable, Category = "Network")
	void OnSendWebSocketClicked();

	UFUNCTION(BlueprintCallable, Category = "Network")
	void SetURL(const FString& NewURL);

	UFUNCTION(BlueprintCallable)
	void SetButtonsEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable)
	void OnWebSocketConnected();

	UFUNCTION(BlueprintCallable)
	void OnWebSocketDisConnected();
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class ANodejsGameModeBase* GameModeRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ServerURL;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SendJsonButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ConnectWebSocketButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SendWebSocketButton;

};
