// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABCharacterSkillWidget.h"

#include "Character/ABCharacterPlayer.h"
#include "Character/ABSkillActionData.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInstanceDynamic.h"

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
		TB_CoolTime->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	

	TB_CoolTime->SetVisibility(ESlateVisibility::Visible);
	const auto CoolTimeText = FString::Printf(TEXT("%i"), (static_cast<int>(Player->GetSkillActionDataCoolTime()) - static_cast<int>(Player->GetCurrentSkillTimerRate())));
	TB_CoolTime->SetText(FText::FromString(CoolTimeText));
}
