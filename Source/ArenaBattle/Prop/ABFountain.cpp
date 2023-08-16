// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ABFountain.h"

#include "ArenaBattle.h"
#include "Components/StaticMeshComponent.h"

AABFountain::AABFountain()
{
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Water"));

	RootComponent = Body;
	Water->SetupAttachment(Body);
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 132.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01'"));
	if (BodyMeshRef.Object)
	{
		Body->SetStaticMesh(BodyMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WaterMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Plains_Fountain_02.SM_Plains_Fountain_02'"));
	if (WaterMeshRef.Object)
	{
		Water->SetStaticMesh(WaterMeshRef.Object);
	}
}

void AABFountain::BeginPlay()
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::BeginPlay();
	
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AABFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AABFountain::PostNetInit()
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostNetInit();

	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

//void AABFountain::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
//{
//	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
//
//	Super::PreReplication(ChangedPropertyTracker);
//
//	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
//}
//
//bool AABFountain::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
//{
//	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
//
//	bool IsRelevant = Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
//
//	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
//
//	return IsRelevant;
//}
