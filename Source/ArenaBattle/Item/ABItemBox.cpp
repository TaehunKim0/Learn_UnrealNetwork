// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/ABCollision.h"
#include "Interface/ABCharacterItemInterface.h"
#include "Engine/AssetManager.h"
#include "ABItemData.h"
#include "Character/ABCharacterBase.h"
#include "Character/ABCharacterNonPlayer.h"
#include "Player/InventoryComponent.h"

// Sets default values
AABItemBox::AABItemBox()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));
	
	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);
	
	Trigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ArenaBattle/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (EffectRef.Object)
	{
		Effect->SetTemplate(EffectRef.Object);
		Effect->bAutoActivate = false;
	}

	bAlwaysRelevant = true;
}

void AABItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(TEXT("ABItemData"), Assets);
	ensure(0 < Assets.Num());

	int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
	FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
	if (AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}
	Item = Cast<UABItemData>(AssetPtr.Get());
	ensure(Item);

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnOverlapBegin);
}

void AABItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (nullptr == Item)
	{
		Destroy();
		return;
	}

	// Old 로 돌아가기
	
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);

	if(!HasAuthority()) return;

	AABCharacterBase* Character = Cast<AABCharacterBase>(OtherActor);
	if(!OtherActor) return;
	
	if(Character->GetPlayerState())
	{
		Character->MulticastRPCActivateItemEffect(this);
	}
	else
	{
		AABCharacterBase* LocalPlayer = Cast<AABCharacterBase>(GetWorld()->GetFirstPlayerController());
		LocalPlayer->MulticastRPCActivateItemEffect(this);
	}
	
	// RPC
	// 플레이어 캐릭터 -> 이펙트 액터 스폰(오너가 플레이어인)
	// 액터(이펙트 컴포넌트 가진)를 자체 스폰 -> 리플리케이트
	   // Respawn Client -> Spawn Multicast
	   // Spawn은 했는데 연관성이 있는지 없는지 거리로 판단 
	
	// IABCharacterItemInterface* OverlappingPawn = Cast<IABCharacterItemInterface>(OtherActor);
	// if (OverlappingPawn)
	// {
	// 	OverlappingPawn->TakeItem(Item);
	// }

	TArray<TObjectPtr<UABItemData>> ItemDatas;
	ItemDatas.Add(Item);

	Character->InventoryComponent->AddItems(ItemDatas);
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}

void AABItemBox::ActivateItemEffect()
{
	Effect->Activate(true);
	Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);
}

bool AABItemBox::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	return Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
}
