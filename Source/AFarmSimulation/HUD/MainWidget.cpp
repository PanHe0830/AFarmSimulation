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
	SetVisibility(ESlateVisibility::Visible);/* 设置可见性 */
	//bIsFocusable = true;/* 设置聚焦模式 */

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameAndUI InputModeData;
			//InputModeData.SetHideCursorDuringCapture(false);// 捕获鼠标时不隐藏光标
			PlayerController->SetInputMode(InputModeData);
			//PlayerController->SetShowMouseCursor(true);/* 看到光标 */
		}
	}
}

void UMainWidget::TestButtonFunc()
{
	UE_LOG(LogTemp, Warning, TEXT("World pointer is nullptr"));
	GetOwningPlayer();
}
