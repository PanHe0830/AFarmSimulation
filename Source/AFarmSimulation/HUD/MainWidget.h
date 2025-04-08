// Fill out your copyright notice in the Description page of Project Settings.

/**
*	这个类是游戏主界面的类，它显示个人信息，个人所持有的资源信息等
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

#define _EMPTY_WIDGET_ 0
#define _BUIlD_WIDGET_ 1
#define _HOUSE_WIDGET_ 2

/**
 * 
 */
class UButton;
class UHouseBuildWidget;
class UCanvasPanel;
class UScaleBox;
class USizeBox;
class UWidgetSwitcher;
class UBuildWidget;

UCLASS()
class AFARMSIMULATION_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetMenuWidget();

private:
	UFUNCTION()
	void UseWidgetSwitcherChangeWidgetButtonBuild();

	UFUNCTION()
	void UseWidgetSwitcherChangeWidgetButtonAssert();

private:
	void ChangeUserWidgetIndex(int index);

public:
	// 存放建造物的UI界面
	UPROPERTY(EditAnywhere, Category = "ChildWidget");
	TSubclassOf<UBuildWidget> BuildWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonBuild;

	int CurrentWidgetIndex; // 当前窗口的下标

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonAssert;
};
