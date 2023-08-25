// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ABGameState.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;;
	
public:
	UFUNCTION()
	void OnRep_ConnectedPlayerCount();

	UFUNCTION()
	void OnRep_GameStart();

	UFUNCTION()
	void OnRep_GameClear();

public:
	void OnPlayerScoreChanged(int32 NewPlayerKillScore);

public:
	UPROPERTY(ReplicatedUsing = OnRep_ConnectedPlayerCount)
	int32 ConnectedPlayerCount = 0;

	const int32 GameClearKillScore = 4;

	UPROPERTY(ReplicatedUsing = OnRep_GameStart)
	bool bIsGameStarted = false;

	UPROPERTY(ReplicatedUsing = OnRep_GameClear)
	bool bIsGameClear = false;
};