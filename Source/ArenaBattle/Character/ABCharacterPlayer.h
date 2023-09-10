// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABSkillActionData.h"
#include "Character/ABCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/ABCharacterHUDInterface.h"
#include "ABCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABCharacterPlayer : public AABCharacterBase, public IABCharacterHUDInterface
{
	GENERATED_BODY()
	
public:
	AABCharacterPlayer();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostNetInit() override;
	virtual void OnRep_PlayerState() override;
	// virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	// virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;
	
	virtual void SetDead() override;

public:
	UFUNCTION()
	void OnDeadMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void SetRespawn();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// void SetNameTagWidget();
	
// Character Control Section
protected:
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData) override;

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// UI Widget Section
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> NameTag;

// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SkillAction;

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

	UPROPERTY()
	class UABAnimInstance* MyAnim;

protected:
	void Attack();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCAttack();

// Skill Action
public:
	UFUNCTION(BlueprintCallable)
	float GetSkillActionDataCoolTime() { return SkillActionData->SkillCoolTime; }
	
	UFUNCTION(BlueprintCallable)
	float GetCurrentSkillTimerRate() { return GetWorldTimerManager().GetTimerElapsed(SkillTimerHandle); }

	UPROPERTY(BlueprintReadOnly)
	bool bCanSkillAttack = true; // 스킬 시전 가능여부
	
protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCSkillAttack(); // 스킬 시전 서버 RPC

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCSkillAttack(); // 스킬 시전 멀티캐스트 RPC

	void SkillAttack(); // 스킬 발동
	void SkillEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded); //스킬 시전 종료 (몽타주 엔드 델리게이트에 바인딩함)
	void SetSkillTimer(); // 스킬 쿨타임 시작
	void SetEffectiveSkillAttack(); // 스킬 쿨타임 완료시 스킬 시전가능 상태로 변환
	
	FTimerHandle SkillTimerHandle; // 스킬 쿨타임 타이머 핸들
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABSkillActionData> SkillActionData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> SkillActionMontage;

// UI Section
protected:
	virtual void SetupHUDWidget(class UABHUDWidget* InHUDWidget) override;


};

