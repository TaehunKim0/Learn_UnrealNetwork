// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABCharacterSkillWidget.h"

#include "Character/ABCharacterPlayer.h"
#include "Character/ABSkillActionData.h"
#include "Components/ProgressBar.h"

void UABCharacterSkillWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UABCharacterSkillWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	AABCharacterPlayer* Player = Cast<AABCharacterPlayer>(GetOwningPlayerPawn());

	if (Player->bCanSkillAttack)
	{
		PB_Skill->SetPercent(0);
		return;
	}
	
	PB_Skill->SetPercent(1 - (Player->GetCurrentSkillTimerRate() /Player->GetSkillActionData()->SkillCoolTime));
}
