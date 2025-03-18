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
	// ��ʼ�����λ�ú���ת
	TopDownCameraLocation = FVector(0.0f, 0.0f, 1000.0f); // ���ӽ����λ��
	TopDownCameraRotation = FRotator(0.0f, 0.0f, 0.0f);  // ���ӽ������ת

	// ��ȡ��ɫ��������
	CharacterMesh = GetMesh();

	bIsTopDownView = false;
}

void AAFarmSimulationCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// ����Ĭ�ϵĵ����˳����λ�ú���ת
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
	FVector2D MovementVector = Value.Get<FVector2D>(); // ��ȡ����ֵ��WASD ���ֱ�ҡ�ˣ�

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
		// �����ƶ�����
		//FVector ForwardVector = FVector(1.0f, 0.0f, 0.0f); // ǰ���ƶ�
		//FVector RightVector = FVector(0.0f, 1.0f, 0.0f);   // �����ƶ�

		const FRotator Rotation = GetCameraBoom()->GetRelativeRotation();
		//UE_LOG(LogTemp, Warning, TEXT("Move Rotation is %s"), *Rotation.ToString());
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		// �ƶ����
		FVector NewLocation = GetCameraBoom()->GetComponentLocation() +
			(ForwardDirection * MovementVector.Y * 10.0f) +
			(RightDirection * MovementVector.X * 10.0f);
		
		// �������λ��
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
		// �����������ת
		TopDownCameraRotation.Yaw += LookAxisVector.X; // ˮƽ��ת��Yaw��
		TopDownCameraRotation.Pitch = FMath::Clamp(TopDownCameraRotation.Pitch + LookAxisVector.Y, -89.0f, 89.0f); // ��ֱ��ת��Pitch��

		// ��ӡ�������ת�������ã�
		//UE_LOG(LogTemp, Warning, TEXT("Change TopDownCameraRotation: %s"), *TopDownCameraRotation.ToString());
	}
}

void AAFarmSimulationCharacter::ViewChange()
{
	// �л��ӽ�
	bIsTopDownView = !bIsTopDownView;

	if (bIsTopDownView)
	{
		// �л������ӽ�
		GetCameraBoom()->SetRelativeLocation(TopDownCameraLocation);
		GetCameraBoom()->SetRelativeRotation(TopDownCameraRotation);

		// ���ؽ�ɫģ��
		CharacterMesh->SetVisibility(false);

		// ���ý�ɫ�ƶ�����ѡ��
		GetCharacterMovement()->DisableMovement();
		//UE_LOG(LogTemp, Warning, TEXT("TopDownCameraRotation is %s"), *TopDownCameraRotation.ToString());
	}
	else
	{
		// �л��ص����˳��ӽ�
		GetCameraBoom()->SetRelativeLocation(DefaultCameraLocation);
		GetCameraBoom()->SetRelativeRotation(DefaultCameraRotation);

		// ��ʾ��ɫģ��
		CharacterMesh->SetVisibility(true);

		// ���ý�ɫ�ƶ�����ѡ��
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		//UE_LOG(LogTemp, Warning, TEXT("DefaultCameraRotation is %s"), *DefaultCameraRotation.ToString());
	}
}