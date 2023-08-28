// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABLobbyWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Game/ABGameMode.h"
#include "Game/ABGameState.h"
#include "Player/ABPlayerController.h"

void UABLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UABLobbyWidget::UpdatePlayerCount(int32 InCount)
{
	FText Text = FText::FromString(FString::Printf(TEXT("접속한 플레이어 : %d / 4"), InCount));
	PlayerCountText->SetText(Text);
}

void UABLobbyWidget::SetIsClient(bool bNewIsClient)
{
	bIsHost = !bNewIsClient;

	if(bIsHost)
	{
		Button->OnClicked.AddDynamic(this, &UABLobbyWidget::GameStart);
	}
	else
	{
		Button->OnClicked.AddDynamic(this, &UABLobbyWidget::Join);
		ButtonText->SetText(FText::FromString(TEXT("게임 접속")));
	}
}

void UABLobbyWidget::GameStart()
{
	UWorld* World = GetWorld();
	AABGameMode * GameMode = Cast<AABGameMode>(World->GetAuthGameMode());
	if(GameMode)
	{
		if(GameMode->GetPlayerCount() >= 4)
		{
			GameMode->GameStart();
			RemoveFromParent();
			World->GetFirstPlayerController()->SetShowMouseCursor(false);
			World->ServerTravel("/Game/ArenaBattle/Maps/PVP?listen");
		}
		else
		{
			NoticeText->SetText(FText::FromString(TEXT("플레이어가 부족합니다.")));
		}
	}
}

void UABLobbyWidget::Join()
{
	AABPlayerController* PC = Cast<AABPlayerController>(GetOwningPlayer());
	if(PC) PC->ServerRPCConnectPlayer();
	
	Button->SetIsEnabled(false);
	NoticeText->SetText(FText::FromString(TEXT("게임에 접속했습니다.")));
}