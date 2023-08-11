// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameMode.h"
#include "ABGameMode.h"

#include "Character/ABCharacterBase.h"
#include "Player/ABPlayerController.h"

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
	Super::PostLogin(NewPlayer);
}

void AABGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("PostLogin"));

	AABCharacterBase* PlayerCharacter = Cast<AABCharacterBase>(NewPlayer->GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->SetCharacterAppearIndex(CurrentAppearIndex);
		CurrentAppearIndex++;
		CurrentPlayerNum++;
	}
	else
	{
		checkf(true, TEXT("PlayerCharacter is nullptr"));
	}

	if(CurrentPlayerNum >= MaxPlayerNum)
	{
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			AABCharacterBase* ABCharacter = Cast<AABCharacterBase>(Iterator->Get()->GetPawn());
			if (ABCharacter)
			{
				UE_LOG(LogTemp, Warning, TEXT("Set Character Appearance : %d"), ABCharacter->AppearIndex);
				ABCharacter->MulticastRPCSetCharacterAppearance(ABCharacter->AppearIndex);
			}
			else
			{
				checkf(true, TEXT("ABCharacter is nullptr"));
			}
		}
	}
}
