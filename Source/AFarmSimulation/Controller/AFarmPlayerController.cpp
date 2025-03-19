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

    bIsTopDownView = true; // 初始状态为俯视角
}

void AAFarmPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 在世界中创建俯视摄像机
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    TopDownCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), SpawnParams);

    if (TopDownCamera)
    {
        CameraStartLocation = FVector(0, 0, 1500); // 俯视角初始位置
        TopDownCamera->SetActorLocation(CameraStartLocation);
        TopDownCamera->SetActorRotation(FRotator(-90, 0, 0)); // 俯视角朝向地面
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
        // 切换回第三人称
        ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        if (PlayerCharacter)
        {
            SetViewTarget(PlayerCharacter);
        }
    }
    else
    {
        // 切换到俯视角
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