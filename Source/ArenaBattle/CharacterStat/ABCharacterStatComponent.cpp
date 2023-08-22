// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"

#include "ArenaBattle.h"
#include "Character/ABCharacterPlayer.h"
#include "Components/WidgetComponent.h"
#include "GameData/ABGameSingleton.h"
#include "Net/UnrealNetwork.h"
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
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
		const AABCharacterBase* AttackActor = Cast<AABCharacterBase>(Attacker);
		if (AttackActor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("킬스코어 올림!")));
			Cast<AABPlayerState>(AttackActor->GetPlayerState())->KillScore += 1;

			//서버코드
			if(UUserWidget* NameTagClass = Cast<AABCharacterPlayer>(AttackActor)->NameTag->GetWidget())
			{
				Cast<UABNameTagWidget>(NameTagClass)->UpdatePlayerNameTag();
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("어태커 없음")));
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
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("사망!")));
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

