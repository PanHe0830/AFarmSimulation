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
	//UE_LOG(LogTemp, Warning, TEXT("World pointer is nullptr"));
	// ȷ�� PopupWidgetClass ������
	if (HouseWidgetClass)
	{
		// ���� PopupWidget ʵ��
		HouseWidgetInstance = CreateWidget<UHouseBuildWidget>(GetWorld(), HouseWidgetClass);
		if (HouseWidgetInstance)
		{
			// ��ӵ��ӿ�
			HouseWidgetInstance->AddToViewport();

			// ���� PopupWidget ��λ�úʹ�С
			//FVector2D Position(0, 300); // ����λ��
			FVector2D Position = GetCanvasPanelLocation();
			Position.Y = Position.Y - 90.f;
			UE_LOG(LogTemp , Warning , TEXT("position %s") , *Position.ToString());
			//FVector2D Size(300, 200);    // ���ô�С
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
		// ��ȡ CanvasPanel �ļ�����Ϣ
		FGeometry Geometry = CanvasPanel->GetCachedGeometry();

		// ��ȡ CanvasPanel �Ĵ�С
		FVector2D Size = Geometry.GetLocalSize();

		// ���½ǵľֲ�����
		FVector2D LocalBottomLeft(0, Size.Y);

		// ���ֲ�����ת��Ϊ��Ļ����
		FVector2D ScreenPosition = Geometry.LocalToAbsolute(LocalBottomLeft);

		// ������Ļ����
		return ScreenPosition;
	}
#endif

	if (CanvasPanel)
	{
		// ��ȡ CanvasPanel �ļ�����Ϣ
		FGeometry Geometry = CanvasPanel->GetCachedGeometry();

		// ��ȡ CanvasPanel �Ĵ�С
		FVector2D OutSize = Geometry.GetLocalSize();
		OutSize.X = 0.f;

		// ��ȡ CanvasPanel ����Ļλ�ã����Ͻǣ�
		FVector2D OutPosition = Geometry.LocalToAbsolute(FVector2D::ZeroVector);

		return OutSize;
	}

	return FVector2D::ZeroVector;
}
