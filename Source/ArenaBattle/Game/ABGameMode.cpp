// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameMode.h"
#include "ABGameMode.h"

#include "ArenaBattle.h"
#include "Character/ABCharacterBase.h"
#include "GameFramework/PlayerState.h"
#include "Player/ABPlayerController.h"
#include "Player/ABPlayerState.h"

AABGameMode::AABGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/ArenaBattle/Blueprint/BP_ABCharacterPlayer.BP_ABCharacterPlayer_C'"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ArenaBattle.ABPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	PlayerStateClass = AABPlayerState::StaticClass();

	ClearScore = 3;
	CurrentScore = 0;
	bIsCleared = false;
}

void AABGameMode::OnPlayerScoreChanged(int32 NewPlayerScore)
{
	CurrentScore = NewPlayerScore;

	AABPlayerController* ABPlayerController = Cast<AABPlayerController>(GetWorld()->GetFirstPlayerController());
	if (ABPlayerController)
	{
		ABPlayerController->GameScoreChanged(CurrentScore);
	}

	if (CurrentScore >= ClearScore)
	{
		bIsCleared = true;

		if (ABPlayerController)
		{
			ABPlayerController->GameClear();
		}
	}
}

void AABGameMode::OnPlayerDead()
{
	AABPlayerController* ABPlayerController = Cast<AABPlayerController>(GetWorld()->GetFirstPlayerController());
	if (ABPlayerController)
	{
		ABPlayerController->GameOver();
	}
}

bool AABGameMode::IsGameCleared()
{
	return bIsCleared;
}

void AABGameMode::PostLogin(APlayerController* NewPlayer)
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostLogin(NewPlayer);

	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"))

	AABCharacterBase* PlayerCharacter = Cast<AABCharacterBase>(NewPlayer->GetPawn());
	if (PlayerCharacter)
	{
		PlayerInfos.Add({PlayerCharacter->GetPlayerState()->GetPlayerId()});

		PlayerCharacter->Stat.Get()->AppearIndex = 	AppearanceCount;
		//PlayerCharacter->GetPlayerState()->SetPlayerName(FString::Printf(TEXT("플레이어 %i"), PlayerCharacter->GetPlayerState()->GetPlayerId()));

		AppearanceCount++;
		if (AppearanceCount >= 4) AppearanceCount = 0;
		PlayerCharacter->Stat->SetCharacterAppearance();
	}
}

void AABGameMode::GameWelcomePlayer(UNetConnection* Connection, FString& RedirectURL)
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::GameWelcomePlayer(Connection, RedirectURL);

	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

APlayerController* AABGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
	const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));

	return NewPlayerController;
}

void AABGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

FString AABGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
	const FString& Options, const FString& Portal)
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
	FString NewPlayerString = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	auto state = NewPlayerController->GetPlayerState<APlayerState>();
	AB_LOG(LogABNetwork, Log, TEXT("%d"), state->GetPlayerId());
	
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
	
	return NewPlayerString;
}
