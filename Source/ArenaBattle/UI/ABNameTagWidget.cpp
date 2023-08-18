// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABNameTagWidget.h"

#include "ArenaBattle.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"

void UABNameTagWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UpdatePlayerNameTag();
}

void UABNameTagWidget::UpdatePlayerNameTag()
{
	APlayerState* PlayerState = Cast<ACharacter>(OwningActor)->GetPlayerState();
	
	if(PlayerState)
	{
		FString ID = FString::Printf(TEXT("%d"), PlayerState->GetPlayerId());
			
		FText PlayerIDString = FText::FromString(ID);
		PlayerName->SetText(PlayerIDString);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어 스테이트 없음!"));
	}
}
