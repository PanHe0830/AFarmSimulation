// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AFarmPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AFARMSIMULATION_API AAFarmPlayerController : public APlayerController
{
	GENERATED_BODY()

#if 0
public:
    AAFarmPlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    // 视角切换
    void ToggleViewMode();

    // 移动控制
    void MoveForward(float Value);
    void MoveRight(float Value);

private:
    bool bIsTopDownView;  // 是否处于俯视角模式
    UPROPERTY(VisibleAnywhere, Category = "Camera")
    class ACameraActor* TopDownCamera; // 俯视摄像机
    FVector CameraStartLocation; // 记录初始位置

#endif
};
