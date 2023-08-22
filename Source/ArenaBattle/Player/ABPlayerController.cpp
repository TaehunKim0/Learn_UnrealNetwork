// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"

#include "ABPlayerState.h"
#include "UI/ABHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ABSaveGame.h"
#include "ArenaBattle.h"
#include "Character/ABCharacterPlayer.h"
#include "Game/ABGameState.h"
#include "UI/ABLobbyWidget.h"
#include "GameFramework/PlayerState.h"

class AABGameState;
DEFINE_LOG_CATEGORY(LogABPlayerController);

AABPlayerController::AABPlayerController()
{
	static ConstructorHelpers::FClassFinder<UABHUDWidget> ABHUDWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_ABHUD.WBP_ABHUD_C"));
	if (ABHUDWidgetRef.Class)
	{
		ABHUDWidgetClass = ABHUDWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> LobbyWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_Lobby.WBP_Lobby_C"));
	if(LobbyWidgetRef.Class)
	{
		LobbyWidgetClass = LobbyWidgetRef.Class;	
	}
}

void AABPlayerController::PlayerCountChanged(int32 NewPlayerCount)
{
	if(LobbyWidget)
	{
		LobbyWidget->UpdatePlayerCount(NewPlayerCount);
	}
}

void AABPlayerController::GameScoreChanged(int32 NewScore)
{
	K2_OnScoreChanged(NewScore);
}

void AABPlayerController::GameClear()
{
	K2_OnGameClear();
}

void AABPlayerController::GameOver()
{
	K2_OnGameOver();

	if (!UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Player0"), 0))
	{
		UE_LOG(LogABPlayerController, Error, TEXT("Save Game Error!"));
	}

	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
}

void AABPlayerController::SetPlayer(UPlayer* InPlayer)
{
	AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::SetPlayer(InPlayer);

	AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AABPlayerController::ReceivedPlayer()
{
	AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::ReceivedPlayer();

	AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AABPlayerController::PostNetInit()
{
	AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostNetInit();

	AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AABPlayerController::SetPawn(APawn* InPawn)
{
	if (InPawn)
	{
		AB_PCLOG(LogABNetwork, Log, TEXT("%s [%s]"), TEXT("Begin: InPawn's Name "), *InPawn->GetName());
	}
	else
	{
		AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin: InPawn is nullptr"));
	}

	Super::SetPawn(InPawn);

	if (InPawn)
	{
		AB_PCLOG(LogABNetwork, Log, TEXT("%s [%s]"), TEXT("End: InPawn's Name "), *InPawn->GetName());
	}
	else
	{
		AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin: InPawn is nullptr"));
	}
}

void AABPlayerController::OnRep_PlayerState()
{
	AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnRep_PlayerState();

	AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AABPlayerController::PostNetReceive()
{
	AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
	Super::PostNetReceive();
	AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AABPlayerController::ServerRPCConnectPlayer_Implementation()
{
	ConnectPlayer();
}

void AABPlayerController::ConnectPlayer()
{
	UWorld *World = GetWorld();
	AABGameState* GameState = Cast<AABGameState>(World->GetGameState());
	if(GameState == nullptr) return;

	AABPlayerController* PC = Cast<AABPlayerController>(World->GetFirstPlayerController());
	if(PC)
	{
		GameState->ConnectedPlayerCount++; //OnRep
		PC->PlayerCountChanged(GameState->ConnectedPlayerCount);
	}
}

void AABPlayerController::RemoveLobbyWidget()
{
	LobbyWidget->RemoveFromParent();
	UE_LOG(LogABPlayerController, Log, TEXT("RemoveLobbyWidget"));
}

void AABPlayerController::OnPossess(APawn* aPawn)
{
	AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnPossess(aPawn);
	// Cast<AABCharacterPlayer>(aPawn)->SetNameTagWidget();

	AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AABPlayerController::BeginPlay()
{
	AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
	Super::BeginPlay();
	AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	SaveGameInstance = Cast<UABSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Player0"), 0));
	if (SaveGameInstance)
	{
		SaveGameInstance->RetryCount++;
	}
	else
	{
		SaveGameInstance = NewObject<UABSaveGame>();
		SaveGameInstance->RetryCount = 0;
	}

	K2_OnGameRetryCount(SaveGameInstance->RetryCount);

// Widget
	if(IsLocalController())
	{
		LobbyWidget = CreateWidget<UABLobbyWidget>(GetWorld(), LobbyWidgetClass);
		if(LobbyWidget == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("LobbyWidget is nullptr"));
			return;
		}
		if(HasAuthority())
			LobbyWidget->SetIsClient(false);
		else
			LobbyWidget->SetIsClient(true);

		LobbyWidget->AddToViewport();
		SetShowMouseCursor(true);

		if(HasAuthority())
		{
			ConnectPlayer();
		}
	}
}