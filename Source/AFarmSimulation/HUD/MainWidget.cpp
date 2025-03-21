// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Components/Button.h"
#include "HouseBuildWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Components/ScaleBox.h"

bool UMainWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

#if 0
	if (TestButton)
	{
		TestButton->OnClicked.AddDynamic(this,&UMainWidget::TestButtonFunc);
	}
#endif

	if (Button_92)
	{
		Button_92->OnClicked.AddDynamic(this,&UMainWidget::ShowChildWidget);
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
	//UE_LOG(LogTemp, Warning, TEXT("World pointer is nullptr"));
	// 确保 PopupWidgetClass 已设置
	if (HouseWidgetClass)
	{
		// 创建 PopupWidget 实例
		HouseWidgetInstance = CreateWidget<UHouseBuildWidget>(GetWorld(), HouseWidgetClass);
		if (HouseWidgetInstance)
		{
			// 添加到视口
			HouseWidgetInstance->AddToViewport();

			// 设置 PopupWidget 的位置和大小
			//FVector2D Position(0, 300); // 设置位置
			FVector2D Position = GetCanvasPanelLocation();
			Position.Y = Position.Y - 90.f;
			UE_LOG(LogTemp , Warning , TEXT("position %s") , *Position.ToString());
			//FVector2D Size(300, 200);    // 设置大小
			HouseWidgetInstance->SetPositionInViewport(Position);
			//HouseWidgetInstance->SetDesiredSizeInViewport(Size);
		}
	}
	
}

void UMainWidget::ShowChildWidget()
{
	if (HouseWidgetClass && ScaleBox_0)
	{
		HouseWidgetInstance = CreateWidget<UHouseBuildWidget>(this, HouseWidgetClass);
		if (HouseWidgetInstance)
		{
			ScaleBox_0->AddChild(HouseWidgetInstance);
		}
	}
}

FVector2D UMainWidget::GetCanvasPanelLocation()
{
#if 0
	if (CanvasPanel)
	{
		// 获取 CanvasPanel 的几何信息
		FGeometry Geometry = CanvasPanel->GetCachedGeometry();

		// 获取 CanvasPanel 的大小
		FVector2D Size = Geometry.GetLocalSize();

		// 左下角的局部坐标
		FVector2D LocalBottomLeft(0, Size.Y);

		// 将局部坐标转换为屏幕坐标
		FVector2D ScreenPosition = Geometry.LocalToAbsolute(LocalBottomLeft);

		// 返回屏幕坐标
		return ScreenPosition;
	}
#endif

	if (CanvasPanel)
	{
		// 获取 CanvasPanel 的几何信息
		FGeometry Geometry = CanvasPanel->GetCachedGeometry();

		// 获取 CanvasPanel 的大小
		FVector2D OutSize = Geometry.GetLocalSize();
		OutSize.X = 0.f;

		// 获取 CanvasPanel 的屏幕位置（左上角）
		FVector2D OutPosition = Geometry.LocalToAbsolute(FVector2D::ZeroVector);

		return OutSize;
	}

	return FVector2D::ZeroVector;
}
