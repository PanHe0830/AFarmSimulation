// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Components/Button.h"

bool UMainWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (TestButton)
	{
		TestButton->OnClicked.AddDynamic(this,&UMainWidget::TestButtonFunc);
	}

	return true;
}

void UMainWidget::SetMenuWidget()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);/* ���ÿɼ��� */
	//bIsFocusable = true;/* ���þ۽�ģʽ */

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameAndUI InputModeData;
			//InputModeData.SetHideCursorDuringCapture(false);// �������ʱ�����ع��
			PlayerController->SetInputMode(InputModeData);
			//PlayerController->SetShowMouseCursor(true);/* ������� */
		}
	}
}

void UMainWidget::TestButtonFunc()
{
	UE_LOG(LogTemp, Warning, TEXT("World pointer is nullptr"));
	GetOwningPlayer();
}
