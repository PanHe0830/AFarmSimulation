// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Components/Button.h"
#include "HouseBuildWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Components/ScaleBox.h"
#include "Components/WidgetSwitcher.h"
#include "BuildWidget.h"

bool UMainWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	CurrentWidgetIndex = _EMPTY_WIDGET_;

	if (ButtonBuild)
	{
		ButtonBuild->OnClicked.AddDynamic(this, &UMainWidget::UseWidgetSwitcherChangeWidgetButtonBuild);
	}

	if (ButtonAssert)
	{
		ButtonAssert->OnClicked.AddDynamic(this, &UMainWidget::UseWidgetSwitcherChangeWidgetButtonAssert);
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

void UMainWidget::UseWidgetSwitcherChangeWidgetButtonBuild()
{
	if (WidgetSwitcher)
	{
		ChangeUserWidgetIndex(_BUIlD_WIDGET_);
	}
}

void UMainWidget::UseWidgetSwitcherChangeWidgetButtonAssert()
{
	if (WidgetSwitcher)
	{
		ChangeUserWidgetIndex(_HOUSE_WIDGET_);
	}
}

void UMainWidget::ChangeUserWidgetIndex(int index)
{
	if (CurrentWidgetIndex != index)
	{
		WidgetSwitcher->SetActiveWidgetIndex(index);
		CurrentWidgetIndex = index;
	}
	else
	{
		WidgetSwitcher->SetActiveWidgetIndex(_EMPTY_WIDGET_);
		CurrentWidgetIndex = _EMPTY_WIDGET_;
	}
}
