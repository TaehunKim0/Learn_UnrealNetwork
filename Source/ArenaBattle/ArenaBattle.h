// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define LOG_NETMODEINFO ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT%d"), (GetWorld()->WorldType == EWorldType::PIE) ? GPlayInEditorID : 0) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER")))
#define LOG_CALLINFO *(FString(__FUNCTION__))
#define LOG_LOCALROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetLocalRole()))
#define LOG_REMOTEROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetRemoteRole()))
#define LOG_PCLOCALPROXY (IsLocalPlayerController() ? TEXT("LOCAL") : TEXT("PROXY"))
#define LOG_LOCALPROXY (IsLocallyControlled() ? TEXT("LOCAL") : TEXT("PROXY"))
#define LOG_COMPLOCALROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwner()->GetLocalRole()))
#define LOG_COMPREMOTEROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwner()->GetRemoteRole()))
#define LOG_COMPLOCALPROXY (Cast<APawn>(GetOwner()) ? (Cast<APawn>(GetOwner())->IsLocallyControlled() ? TEXT("LOCAL") : TEXT("PROXY")) : TEXT(""))
#define AB_LOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s %s"), LOG_NETMODEINFO, LOG_LOCALROLEINFO, LOG_REMOTEROLEINFO, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define AB_PCLOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s][%s] %s %s"), LOG_NETMODEINFO, LOG_LOCALROLEINFO, LOG_REMOTEROLEINFO, LOG_PCLOCALPROXY, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define AB_PAWNLOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s][%s] %s %s"), LOG_NETMODEINFO, LOG_LOCALROLEINFO, LOG_REMOTEROLEINFO, LOG_LOCALPROXY, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define AB_COMPLOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s][%s] %s %s"), LOG_NETMODEINFO, LOG_COMPLOCALROLEINFO, LOG_COMPREMOTEROLEINFO, LOG_COMPLOCALPROXY, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define AB_STATELOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s] %s %s"), LOG_NETMODEINFO, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

DECLARE_LOG_CATEGORY_EXTERN(LogABNetwork, Log, All);