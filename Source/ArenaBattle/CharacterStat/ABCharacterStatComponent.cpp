// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"

#include "ArenaBattle.h"
#include "Character/ABCharacterPlayer.h"
#include "GameData/ABGameSingleton.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	CurrentLevel = 1;
	AttackRadius = 50.0f;

	bWantsInitializeComponent = true;

	SetIsReplicated(true);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef1(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef2(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Barbarous.SK_CharM_Barbarous'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef3(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_FrostGiant.SK_CharM_FrostGiant'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef4(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Golden.SK_CharM_Golden'"));

	CharacterSkeletalMeshes.Add(CharacterMeshRef1.Object);
	CharacterSkeletalMeshes.Add(CharacterMeshRef2.Object);
	CharacterSkeletalMeshes.Add(CharacterMeshRef3.Object);
	CharacterSkeletalMeshes.Add(CharacterMeshRef4.Object);
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
	DOREPLIFETIME(UABCharacterStatComponent, AppearIndex);
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

float UABCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
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

void UABCharacterStatComponent::SetCharacterAppearance()
{
	const auto PlayerCharacter = Cast<AABCharacterPlayer>(GetOwner());
	PlayerCharacter->GetMesh()->SetSkeletalMesh(CharacterSkeletalMeshes[AppearIndex]);
}

void UABCharacterStatComponent::OnRep_AppearIndex()
{
	SetCharacterAppearance();
}

