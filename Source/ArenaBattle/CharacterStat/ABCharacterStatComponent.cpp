// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"

#include "ArenaBattle.h"
#include "Character/ABCharacterPlayer.h"
#include "Components/WidgetComponent.h"
#include "Game/ABGameMode.h"
#include "Game/ABGameState.h"
#include "GameData/ABGameSingleton.h"
#include "Net/UnrealNetwork.h"
#include "Player/ABPlayerController.h"
#include "Player/ABPlayerState.h"
#include "UI/ABNameTagWidget.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	CurrentLevel = 1;
	AttackRadius = 50.0f;

	bWantsInitializeComponent = true;

	SetIsReplicated(true);
}

void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
}

void UABCharacterStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UABCharacterStatComponent, CurrentHp);
}

void UABCharacterStatComponent::ReadyForReplication()
{
	AB_COMPLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::ReadyForReplication();

	AB_COMPLOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

void UABCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UABGameSingleton::Get().CharacterMaxLevel);
	SetBaseStat(UABGameSingleton::Get().GetCharacterStat(CurrentLevel));
	check(BaseStat.MaxHp > 0.0f);
}

float UABCharacterStatComponent::ApplyDamage(float InDamage, AActor* Attacker)
{
	// 서버에서만 실행됨.
	
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
		const AABCharacterBase* AttackActor = Cast<AABCharacterBase>(Attacker);
		if (AttackActor)
		{	
			AABPlayerState* PlayerState = Cast<AABPlayerState>(AttackActor->GetPlayerState());
			if(PlayerState)
			{
				PlayerState->KillScore += 1;

				UWorld* World = GetWorld();
				AABGameState* GameState =  Cast<AABGameState>(World->GetGameState());
				if(GameState)
				{
					GameState->OnPlayerScoreChanged(PlayerState->KillScore);
				}
			}

			if(UUserWidget* NameTagClass = Cast<AABCharacterPlayer>(AttackActor)->NameTag->GetWidget())
			{
				Cast<UABNameTagWidget>(NameTagClass)->UpdatePlayerNameTag();
			}
		}
	}

	return ActualDamage;
}

void UABCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);
	
	OnHpChanged.Broadcast(CurrentHp);
}

void UABCharacterStatComponent::OnRep_CurrentHp() const
{
	OnHpChanged.Broadcast(CurrentHp);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
}

// void UABCharacterStatComponent::SetCharacterAppearance()
// {
// 	const auto PlayerCharacter = Cast<AABCharacterPlayer>(GetOwner());
// 	PlayerCharacter->GetMesh()->SetSkeletalMesh(CharacterSkeletalMeshes[AppearIndex]);
// }
//
// void UABCharacterStatComponent::OnRep_AppearIndex()
// {
// 	SetCharacterAppearance();
// }

