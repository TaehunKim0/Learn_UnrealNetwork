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
	
	UPROPERTY(ReplicatedUsing = OnRep_ConnectedPlayerCount)
	int32 ConnectedPlayerCount = 0;

	UPROPERTY(ReplicatedUsing = OnRep_GameStart)
	bool bIsGameStarted = false;
};
