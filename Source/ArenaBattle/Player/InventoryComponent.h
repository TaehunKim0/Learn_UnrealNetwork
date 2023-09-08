// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/ActorChannel.h"
#include "Item/ABItemData.h"
#include "Net/UnrealNetwork.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();
	UInventoryComponent(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		// Component must be replicated to replicate sub-objects
		SetIsReplicatedByDefault(true);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UInventoryComponent, Items);
	}

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override
	{
		bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
		bWroteSomething |= Channel->ReplicateSubobjectList(Items , *Bunch, *RepFlags);
		return bWroteSomething;
	}

	void AddItems(const TArray<TObjectPtr<UABItemData>> InItemDatas)
	{
		if(InItemDatas.IsEmpty()) return;
		Items.Reset();

		for(auto& ItemData : InItemDatas)
		{
			Items.Add(ItemData);
		}

		UseItem(Items);
	}

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UseItem(TArray<UABItemData*> InItemDatas);
	
	UFUNCTION()
	void OnRep_Items();
	
private:
	UPROPERTY(ReplicatedUsing= OnRep_Items)
	TArray<UABItemData*> Items;
};