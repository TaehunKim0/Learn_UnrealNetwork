// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerState.h"
#include "ArenaBattle.h"

AABPlayerState::AABPlayerState()
{
	AB_STATELOG(LogABNetwork, Log, TEXT("%s"), TEXT("Create"));
}

void AABPlayerState::BeginPlay()
{
	Super::BeginPlay();
	AB_STATELOG(LogABNetwork, Log, TEXT("%s"), *GetPlayerName());
}

void AABPlayerState::OnRep_PlayerId()
{
	Super::OnRep_PlayerId();
}

void AABPlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();
}

void AABPlayerState::PostNetInit()
{
	Super::PostNetInit();
}

void AABPlayerState::PostNetReceive()
{
	Super::PostNetReceive();
}
