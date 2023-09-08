// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/InventoryComponent.h"

#include "Character/ABCharacterBase.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::UseItem(TArray<UABItemData*> InItemDatas)
{
	AABCharacterBase* ABCharacterBase = Cast<AABCharacterBase>(GetOwner());
	ABCharacterBase->UseInventory(Items);
}

void UInventoryComponent::OnRep_Items()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Items"));
	UseItem(Items);
}

