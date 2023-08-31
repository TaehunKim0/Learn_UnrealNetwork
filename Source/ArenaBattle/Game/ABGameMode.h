// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Interface/ABGameInterface.h"
#include "ABGameMode.generated.h"


USTRUCT()
struct FPlayerInfo
{
	GENERATED_BODY()
	int32 PlayerID;
};


UCLASS()
class ARENABATTLE_API AABGameMode : public AGameMode, public IABGameInterface
{
	GENERATED_BODY()
	
public:
	AABGameMode();

	virtual void OnPlayerScoreChanged(int32 NewPlayerScore) override;
	virtual void OnPlayerDead() override;
	virtual bool IsGameCleared() override;

	void GameStart();
	void JoinPlayer();
	void BackToLobby();
	int32 GetPlayerCount() const;
	
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void GameWelcomePlayer(UNetConnection* Connection, FString& RedirectURL) override;
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT("")) override;

	virtual void StartPlay() override;
	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 ClearScore;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	int32 CurrentScore;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	uint8 bIsCleared : 1;

	int32 AppearanceCount = 0;
	
	UPROPERTY()
	TArray<FPlayerInfo> PlayerInfos;

private:
	FTimerHandle BackToLobbyTimerHandle;
};