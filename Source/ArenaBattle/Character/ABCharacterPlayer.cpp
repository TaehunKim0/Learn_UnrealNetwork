// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ABCharacterControlData.h"
#include "ABSkillActionData.h"
#include "ArenaBattle.h"
#include "Animation/ABAnimInstance.h"
#include "Blueprint/WidgetTree.h"
#include "UI/ABHUDWidget.h"
#include "CharacterStat/ABCharacterStatComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Interface/ABGameInterface.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerState.h"
#include "Player/ABPlayerState.h"
#include "UI/ABNameTagWidget.h"
#include "UI/ABWidgetComponent.h"

AABCharacterPlayer::AABCharacterPlayer()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Widget Component 
	NameTag = CreateDefaultSubobject<UABWidgetComponent>(TEXT("NameTag"));
	NameTag->SetupAttachment(GetMesh());
	NameTag->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_NameTag.WBP_NameTag_C"));
	if (HpBarWidgetRef.Class)
	{
		NameTag->SetWidgetClass(HpBarWidgetRef.Class);
		NameTag->SetWidgetSpace(EWidgetSpace::Screen);
		NameTag->SetDrawSize(FVector2D(150.0f, 15.0f));
		NameTag->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ChangeControl.IA_ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSkillRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Skill.IA_Skill'"));
	if (nullptr != InputActionSkillRef.Object)
	{
		SkillAction = InputActionSkillRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UABSkillActionData> SkillActionDataRef(TEXT("/Script/ArenaBattle.ABComboActionData'/Game/ArenaBattle/CharacterAction/ABA_SkillAttack.ABA_SkillAttack'"));
	if (SkillActionDataRef.Object)
	{
		SkillActionData = SkillActionDataRef.Object;
	}

	CurrentCharacterControlType = ECharacterControlType::Quater;
}

void AABCharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MyAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	if (!ensure(MyAnim != nullptr)) return;
	MyAnim->OnMontageEnded.AddDynamic(this, &AABCharacterPlayer::OnDeadMontageEnded);
}

void AABCharacterPlayer::BeginPlay()
{
	AB_PAWNLOG(LogABNetwork, Log, TEXT("[%s] %s"), *GetName(), TEXT("Begin"));
	Super::BeginPlay();
	AB_PAWNLOG(LogABNetwork, Log, TEXT("[%s] %s"), *GetName(), TEXT("End"));
	
	if(false == IsLocallyControlled())
		return;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	SetCharacterControl(CurrentCharacterControlType);
}

void AABCharacterPlayer::SetDead()
{
	Super::SetDead();

	// if(false == IsLocallyControlled())
	// 	return;

	// if (!HasAuthority())
	// {
	// 	return;
	// }
	//
	// APlayerController* PlayerController = Cast<APlayerController>(GetController());
	// if (PlayerController)
	// {
	// 	DisableInput(PlayerController);
	//
	// 	IABGameInterface* ABGameMode = Cast<IABGameInterface>(GetWorld()->GetAuthGameMode());
	// 	if (ABGameMode)
	// 	{
	// 		ABGameMode->OnPlayerDead();
	// 	}
	// }
}

void AABCharacterPlayer::SetRespawn()
{
	Stat->HealHp(Stat->GetTotalStat().MaxHp);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	SetActorEnableCollision(true);
	HpBar->SetHiddenInGame(false);
	SetActorLocation(FVector(0,0,210));
}

void AABCharacterPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AABCharacterPlayer::PostNetInit()
{
	AB_PAWNLOG(LogABNetwork, Log, TEXT("[%s] %s"), *GetName(), TEXT("Begin"));

	Super::PostNetInit();

	AB_PAWNLOG(LogABNetwork, Log, TEXT("[%s] %s"), *GetName(), TEXT("End"));
}

void AABCharacterPlayer::OnRep_PlayerState()
{
	AB_PAWNLOG(LogABNetwork, Log, TEXT("[%s] %s"), *GetName(), TEXT("Begin"));

	Super::OnRep_PlayerState();
	
	if(NameTag->GetWidget())
	{
		Cast<UABNameTagWidget>(NameTag->GetWidget())->UpdatePlayerNameTag();
	}

	Cast<AABPlayerState>(GetPlayerState())->SetCharacterAppearance();
	
	AB_PAWNLOG(LogABNetwork, Log, TEXT("[%s] %s"), *GetName(), TEXT("End"));
}

void AABCharacterPlayer::OnDeadMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == DeadMontage)
	{
		SetRespawn();
	}
}

void AABCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::QuaterMove);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Attack);
	EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ServerRPCSkillAttack);
}

void AABCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void AABCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	if(false == IsLocallyControlled())
		return;
	
	UABCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void AABCharacterPlayer::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void AABCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AABCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AABCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void AABCharacterPlayer::Attack()
{
	ServerRPCAttack();
}

// 최근, 악성 데이터/입력 감지를 위한 관문 역할을 위해 RPC 에 인증(validation) 함수를 추가하는 기능이 생겼습니다.
// RPC 에 대한 인증 함수가 악성 파라미터를 감지한 경우, 해당 RPC 를 호출한 클라이언트/서버 연결을 끊도록 시스템에 알린다는 개념입니다. 
bool AABCharacterPlayer::ServerRPCAttack_Validate()
{
	// 인증
	return true;
}

void AABCharacterPlayer::ServerRPCAttack_Implementation()
{
	// if 공격 가능한가?
	MulticastRPCAttack();
}

void AABCharacterPlayer::MulticastRPCAttack_Implementation()
{
	ProcessComboCommand();
}

void AABCharacterPlayer::ServerRPCSkillAttack_Implementation()
{
	if(bCanSkillAttack)
	{
		UE_LOG(LogClass, Log, TEXT("Server 의 쿨타임이 끝남"));
		
		MulticastRPCSkillAttack();
	}
	else
	{
		UE_LOG(LogClass, Log, TEXT("Server 의 쿨타임이 끝나지 않음"));
	}
}

bool AABCharacterPlayer::ServerRPCSkillAttack_Validate()
{
	return true;
}

void AABCharacterPlayer::MulticastRPCSkillAttack_Implementation()
{
	SkillAttack();
}

void AABCharacterPlayer::SkillAttack()
{
	if (!bCanSkillAttack) return;
	
	bCanSkillAttack = false;
	
	// Movement Setting
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Animation Setting
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(SkillActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AABCharacterPlayer::SkillEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, SkillActionMontage);
}

void AABCharacterPlayer::SkillEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	SetSkillTimer();
}

void AABCharacterPlayer::SetSkillTimer()
{
	GetWorld()->GetTimerManager().SetTimer(SkillTimerHandle, this, &AABCharacterPlayer::SetEffectiveSkillAttack, SkillActionData->SkillCoolTime, false);
}

void AABCharacterPlayer::SetEffectiveSkillAttack()
{
	bCanSkillAttack = true;
	SkillTimerHandle.Invalidate();
}

void AABCharacterPlayer::SetupHUDWidget(UABHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		InHUDWidget->UpdateHpBar(Stat->GetCurrentHp());

		Stat->OnStatChanged.AddUObject(InHUDWidget, &UABHUDWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(InHUDWidget, &UABHUDWidget::UpdateHpBar);
	}
}
