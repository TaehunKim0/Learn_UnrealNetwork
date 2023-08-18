// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "ABNameTagWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABNameTagWidget : public UABUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* PlayerName;
};
