// #pragma once
// #include "ABItemData.h"
// #include "Engine/NetSerialization.h"
//
// USTRUCT()
// struct FABItemEntry : public FFastArraySerializerItem
// {
// 	GENERATED_BODY()
//
// 	// Data :
// 	UPROPERTY()
// 	EItemType Type;
//
// 	UPROPERTY()
// 	float FloatProperty;
//
// 	// Func : 
// 	void PreReplicatedRemove();
// 	void PostReplicatedAdd();
// 	void PostReplicatedChange();
// };
//
// USTRUCT()
// struct FABItemArray : public FFastArraySerializer
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY()
// 	TArray<FABItemEntry> Items;
//
// 	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
// 	{
// 		return FastArrayDeltaSerialize<FABItemEntry, FABItemArray>(Items, DeltaParms, *this);
// 	}
// };
//
// template<>
// struct TStructOpsTypeTraits<FABItemArray> : public TStructOpsTypeTraitsBase2<FABItemArray>
// {
// 	enum 
// 	{
// 		WithNetDeltaSerializer = true,
//    };
// };