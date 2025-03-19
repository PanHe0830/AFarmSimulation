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

    // �ӽ��л�
    void ToggleViewMode();

    // �ƶ�����
    void MoveForward(float Value);
    void MoveRight(float Value);

private:
    bool bIsTopDownView;  // �Ƿ��ڸ��ӽ�ģʽ
    UPROPERTY(VisibleAnywhere, Category = "Camera")
    class ACameraActor* TopDownCamera; // ���������
    FVector CameraStartLocation; // ��¼��ʼλ��

#endif
};
