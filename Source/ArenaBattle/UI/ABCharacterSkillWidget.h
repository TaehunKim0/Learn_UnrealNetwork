// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ABUserWidget.h"
#include "ABCharacterSkillWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABCharacterSkillWidget : public UABUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* PB_Skill;
};
