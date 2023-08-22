// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ABUserWidget.h"
#include "ABLobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABLobbyWidget : public UABUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void UpdatePlayerCount(int32 InCount);
	void SetIsClient(bool bNewIsClient);
	
private:
	UFUNCTION()
	void GameStart();
	
	UFUNCTION()
	void Join();
	
public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* PlayerCountText;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* Button;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ButtonText;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* NoticeText;

private:
	bool bIsHost = true;
};
