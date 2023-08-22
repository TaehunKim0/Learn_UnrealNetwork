// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABNameTagWidget.h"

#include "ArenaBattle.h"
#include "Character/ABCharacterBase.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "Player/ABPlayerState.h"

void UABNameTagWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UpdatePlayerNameTag();
}

void UABNameTagWidget::UpdatePlayerNameTag()
{
	AABPlayerState* PlayerState = Cast<AABPlayerState>(Cast<ACharacter>(OwningActor)->GetPlayerState());
	
	if(PlayerState)
	{
		FString KillScore = FString::Printf(TEXT("Score : %d \n"), PlayerState->KillScore);
		FString ID = FString::Printf(TEXT("Name : %d"), PlayerState->GetPlayerId());
			
		FText PlayerIDString = FText::FromString(KillScore + ID);
		PlayerName->SetText(PlayerIDString);
	}
}
