// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABSkillActionData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABSkillActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = SkillData)
	float SkillCoolTime;
};
