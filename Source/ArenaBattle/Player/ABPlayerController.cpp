// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"
#include "UI/ABHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ABSaveGame.h"
#include "ArenaBattle.h"

DEFINE_LOG_CATEGORY(LogABPlayerController);

AABPlayerController::AABPlayerController()
{
	static ConstructorHelpers::FClassFinder<UABHUDWidget> ABHUDWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_ABHUD.WBP_ABHUD_C"));
	if (ABHUDWidgetRef.Class)
	{
		ABHUDWidgetClass = ABHUDWidgetRef.Class;
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

void AABPlayerController::OnActorChannelOpen(FInBunch& InBunch, UNetConnection* Connection)
{
	AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnActorChannelOpen(InBunch, Connection);

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

void AABPlayerController::OnPossess(APawn* aPawn)
{
	AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnPossess(aPawn);

	AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

//bool AABPlayerController::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
//{
//	AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
//
//	bool IsRelevant = Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
//
//	AB_PCLOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
//
//	return IsRelevant;
//}

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
}
