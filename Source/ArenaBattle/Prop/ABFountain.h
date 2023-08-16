// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABFountain.generated.h"

UCLASS()
class ARENABATTLE_API AABFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABFountain();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void PostNetInit() override;
	//virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	//virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> Water;
};