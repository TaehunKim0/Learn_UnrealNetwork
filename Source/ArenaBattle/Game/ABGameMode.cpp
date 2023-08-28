// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameMode.h"
#include "ABGameMode.h"

#include "ABGameState.h"
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
	GameStateClass = AABGameState::StaticClass();

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
		AABGameState * ABGameState = Cast<AABGameState>(GameState);
		if (ABGameState)
		{
			ABGameState->bIsGameClear = true;
			BackToLobby();
		}
		
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

void AABGameMode::GameStart()
{
	AABGameState* State = Cast<AABGameState>(GameState);
	if (State)
	{
		State->bIsGameStarted = true;
	}
}

void AABGameMode::JoinPlayer()
{
	AABGameState* State = Cast<AABGameState>(GameState);
	if (State)
	{
		UE_LOG(LogClass, Log, TEXT("JoinPlayer"));
		State->ConnectedPlayerCount++;
	}
}

void AABGameMode::BackToLobby()
{
	FTimerDelegate TimerDelegate;
	FTimerManager & TimerManager = GetWorldTimerManager();
	UWorld * World = GetWorld();

	UE_LOG(LogClass, Log, TEXT("BackToLobby1"));
	TimerDelegate.BindLambda([World] ()
	{
		UE_LOG(LogClass, Log, TEXT("BackToLobby2"));
		World->ServerTravel("/Game/ArenaBattle/Maps/Lobby?listen");	
	});

	TimerManager.SetTimer(BackToLobbyTimerHandle, TimerDelegate, 5.0f, false);
}

int32 AABGameMode::GetPlayerCount() const
{
	AABGameState* State = Cast<AABGameState>(GameState);
	if (State)
	{
		return State->ConnectedPlayerCount;
	}

	return 0;
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

		AABPlayerState* PlayerState = Cast<AABPlayerState>(PlayerCharacter->GetPlayerState());
		PlayerState->AppearIndex = AppearanceCount;

		AppearanceCount++;
		if (AppearanceCount >= 4) AppearanceCount = 0;
		
		PlayerState->SetCharacterAppearance();
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
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
	
	return NewPlayerString;
}