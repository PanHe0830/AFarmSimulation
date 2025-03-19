// Fill out your copyright notice in the Description page of Project Settings.


#include "AFarmPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "GameFramework/Character.h"

#if 0
AAFarmPlayerController::AAFarmPlayerController()
{
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    bIsTopDownView = true; // ��ʼ״̬Ϊ���ӽ�
}

void AAFarmPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // �������д������������
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    TopDownCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), SpawnParams);

    if (TopDownCamera)
    {
        CameraStartLocation = FVector(0, 0, 1500); // ���ӽǳ�ʼλ��
        TopDownCamera->SetActorLocation(CameraStartLocation);
        TopDownCamera->SetActorRotation(FRotator(-90, 0, 0)); // ���ӽǳ������
        SetViewTarget(TopDownCamera);
    }
}

void AAFarmPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("ToggleView", IE_Pressed, this, &AAFarmPlayerController::ToggleViewMode);
    InputComponent->BindAxis("MoveForward", this, &AAFarmPlayerController::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &AAFarmPlayerController::MoveRight);
}

void AAFarmPlayerController::ToggleViewMode()
{
    if (bIsTopDownView)
    {
        // �л��ص����˳�
        ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        if (PlayerCharacter)
        {
            SetViewTarget(PlayerCharacter);
        }
    }
    else
    {
        // �л������ӽ�
        if (TopDownCamera)
        {
            SetViewTarget(TopDownCamera);
        }
    }

    bIsTopDownView = !bIsTopDownView;
}

void AAFarmPlayerController::MoveForward(float Value)
{
    if (bIsTopDownView && TopDownCamera)
    {
        FVector MoveDirection = FVector(1, 0, 0);
        TopDownCamera->AddActorWorldOffset(MoveDirection * Value * 10);
    }
}

void AAFarmPlayerController::MoveRight(float Value)
{
    if (bIsTopDownView && TopDownCamera)
    {
        FVector MoveDirection = FVector(0, 1, 0);
        TopDownCamera->AddActorWorldOffset(MoveDirection * Value * 10);
    }
}

#endif