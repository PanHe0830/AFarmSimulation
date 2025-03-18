// Copyright Epic Games, Inc. All Rights Reserved.

#include "AFarmSimulationCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AAFarmSimulationCharacter

AAFarmSimulationCharacter::AAFarmSimulationCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// ================================================================================================================
	// 初始化相机位置和旋转
	TopDownCameraLocation = FVector(0.0f, 0.0f, 1000.0f); // 俯视角相机位置
	TopDownCameraRotation = FRotator(0.0f, 0.0f, 0.0f);  // 俯视角相机旋转

	// 获取角色的网格体
	CharacterMesh = GetMesh();

	bIsTopDownView = false;
}

void AAFarmSimulationCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// 保存默认的第三人称相机位置和旋转
	DefaultCameraLocation = GetCameraBoom()->GetRelativeLocation();
	DefaultCameraRotation = GetCameraBoom()->GetRelativeRotation();
	//UE_LOG(LogTemp, Warning, TEXT("DefaultCameraRotation is %s"), *DefaultCameraRotation.ToString());
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAFarmSimulationCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAFarmSimulationCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAFarmSimulationCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

	PlayerInputComponent->BindAction("ViewChange",IE_Pressed,this,&AAFarmSimulationCharacter::ViewChange);
}

void AAFarmSimulationCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>(); // 获取输入值（WASD 或手柄摇杆）

	if (Controller != nullptr && !bIsTopDownView)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
	else
	{
		// 计算移动方向
		//FVector ForwardVector = FVector(1.0f, 0.0f, 0.0f); // 前后移动
		//FVector RightVector = FVector(0.0f, 1.0f, 0.0f);   // 左右移动

		const FRotator Rotation = GetCameraBoom()->GetRelativeRotation();
		//UE_LOG(LogTemp, Warning, TEXT("Move Rotation is %s"), *Rotation.ToString());
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		// 移动相机
		FVector NewLocation = GetCameraBoom()->GetComponentLocation() +
			(ForwardDirection * MovementVector.Y * 10.0f) +
			(RightDirection * MovementVector.X * 10.0f);
		
		// 更新相机位置
		GetCameraBoom()->SetWorldLocation(NewLocation);
	}
}

void AAFarmSimulationCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr && !bIsTopDownView)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
	else
	{
		// 更新摄像机旋转
		TopDownCameraRotation.Yaw += LookAxisVector.X; // 水平旋转（Yaw）
		TopDownCameraRotation.Pitch = FMath::Clamp(TopDownCameraRotation.Pitch + LookAxisVector.Y, -89.0f, 89.0f); // 垂直旋转（Pitch）

		// 打印摄像机旋转（调试用）
		//UE_LOG(LogTemp, Warning, TEXT("Change TopDownCameraRotation: %s"), *TopDownCameraRotation.ToString());
	}
}

void AAFarmSimulationCharacter::ViewChange()
{
	// 切换视角
	bIsTopDownView = !bIsTopDownView;

	if (bIsTopDownView)
	{
		// 切换到俯视角
		GetCameraBoom()->SetRelativeLocation(TopDownCameraLocation);
		GetCameraBoom()->SetRelativeRotation(TopDownCameraRotation);

		// 隐藏角色模型
		CharacterMesh->SetVisibility(false);

		// 禁用角色移动（可选）
		GetCharacterMovement()->DisableMovement();
		//UE_LOG(LogTemp, Warning, TEXT("TopDownCameraRotation is %s"), *TopDownCameraRotation.ToString());
	}
	else
	{
		// 切换回第三人称视角
		GetCameraBoom()->SetRelativeLocation(DefaultCameraLocation);
		GetCameraBoom()->SetRelativeRotation(DefaultCameraRotation);

		// 显示角色模型
		CharacterMesh->SetVisibility(true);

		// 启用角色移动（可选）
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		//UE_LOG(LogTemp, Warning, TEXT("DefaultCameraRotation is %s"), *DefaultCameraRotation.ToString());
	}
}