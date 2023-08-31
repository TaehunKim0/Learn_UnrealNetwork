// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerState.h"
#include "ArenaBattle.h"
#include "Character/ABCharacterPlayer.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "UI/ABNameTagWidget.h"

AABPlayerState::AABPlayerState()
{
	AB_STATELOG(LogABNetwork, Log, TEXT("%s"), TEXT("Create"));
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef1(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef2(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Barbarous.SK_CharM_Barbarous'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef3(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_FrostGiant.SK_CharM_FrostGiant'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef4(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Golden.SK_CharM_Golden'"));

	CharacterSkeletalMeshes.Add(CharacterMeshRef1.Object);
	CharacterSkeletalMeshes.Add(CharacterMeshRef2.Object);
	CharacterSkeletalMeshes.Add(CharacterMeshRef3.Object);
	CharacterSkeletalMeshes.Add(CharacterMeshRef4.Object);

	bGameConnected = false;
}

void AABPlayerState::BeginPlay()
{
	Super::BeginPlay();
	AB_STATELOG(LogABNetwork, Log, TEXT("%s"), *GetPlayerName());
}

void AABPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AABPlayerState, AppearIndex);
	DOREPLIFETIME(AABPlayerState, KillScore);
}

void AABPlayerState::OnRep_PlayerId()
{
	Super::OnRep_PlayerId();
}

void AABPlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();
}

void AABPlayerState::PostNetInit()
{
	Super::PostNetInit();
}

void AABPlayerState::PostNetReceive()
{
	Super::PostNetReceive();
}

void AABPlayerState::OnRep_KillScore()
{
	auto NameTagClass = Cast<AABCharacterPlayer>(GetPawn())->NameTag->GetWidget();

	if(NameTagClass)
	{
		Cast<UABNameTagWidget>(NameTagClass)->UpdatePlayerNameTag();
	}
}

void AABPlayerState::SetCharacterAppearance()
{
	const auto PlayerCharacter = Cast<AABCharacterPlayer>(GetPawn());
	
	if (PlayerCharacter->IsValidLowLevel())
	{
		PlayerCharacter->GetMesh()->SetSkeletalMesh(CharacterSkeletalMeshes[AppearIndex]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어 캐릭터 못가져옴 (외형미적용)"));
	}
}
