// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameState.h"

#include "Net/UnrealNetwork.h"
#include "Player/ABPlayerController.h"

void AABGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AABGameState, ConnectedPlayerCount);
	DOREPLIFETIME(AABGameState, bIsGameStarted);
}

void AABGameState::OnRep_ConnectedPlayerCount()
{
	UWorld * World = GetWorld();
	if(World)
	{
		AABPlayerController* ABPlayerController = Cast<AABPlayerController>(World->GetFirstPlayerController());
		if(ABPlayerController)
		{
			ABPlayerController->PlayerCountChanged(ConnectedPlayerCount);
		}
	}
}

void AABGameState::OnRep_GameStart()
{
	UWorld * World = GetWorld();
	if(World)
	{
		AABPlayerController* ABPlayerController = Cast<AABPlayerController>(World->GetFirstPlayerController());
		if(ABPlayerController)
		{
			ABPlayerController->RemoveLobbyWidget();
			ABPlayerController->SetShowMouseCursor(false);
		}
	}
}
