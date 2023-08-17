// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ABPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AABPlayerState();

	virtual void BeginPlay() override;
	
public:
	virtual void OnRep_PlayerId() override;
	virtual void OnRep_PlayerName() override;
	virtual void PostNetInit() override;
	virtual void PostNetReceive() override;

};
