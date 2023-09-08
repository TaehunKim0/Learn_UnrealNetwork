// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ABReplicateObject.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABReplicateObject : public UObject
{
	GENERATED_BODY()

	// Allows the Object to get a valid UWorld from it's outer.
	virtual UWorld* GetWorld() const override
	{
		if (const UObject* MyOuter = GetOuter())
		{
			return MyOuter->GetWorld();
		}
		return nullptr;
	}
	
	UFUNCTION(BlueprintPure, Category = "My Object")
	AActor* GetOwningActor() const
	{
		return GetTypedOuter<AActor>();
	}
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		// Add any Blueprint properties
		// This is not required if you do not want the class to be "Blueprintable"
		if (const UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass()))
		{
			BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
		}
	}
	
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
	
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override
	{
		check(GetOuter() != nullptr);
		return GetOuter()->GetFunctionCallspace(Function, Stack);
	}
	
	// Call "Remote" (aka, RPC) functions through the actors NetDriver
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, struct FOutParmRec* OutParms, FFrame* Stack) override
	{
		check(!HasAnyFlags(RF_ClassDefaultObject));
		AActor* Owner = GetOwningActor();
		UNetDriver* NetDriver = Owner->GetNetDriver();
		if (NetDriver)
		{
			NetDriver->ProcessRemoteFunction(Owner, Function, Parms, OutParms, Stack, this);
			return true;
		}
		return false;
	}
};
