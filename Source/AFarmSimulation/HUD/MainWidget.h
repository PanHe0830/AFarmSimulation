// Fill out your copyright notice in the Description page of Project Settings.

/**
*	���������Ϸ��������࣬����ʾ������Ϣ�����������е���Դ��Ϣ��
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
class UButton;
class UHouseBuildWidget;
class UCanvasPanel;
class UScaleBox;
class USizeBox;

UCLASS()
class AFARMSIMULATION_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	
public:
	UFUNCTION(BlueprintCallable)
	void SetMenuWidget();

public:
	UPROPERTY(meta = (BindWidget))
	UButton* TestButton;

private:
	UFUNCTION()
	void TestButtonFunc(); 

	UFUNCTION()
	void ShowChildWidget();

	FVector2D GetCanvasPanelLocation();
	
	// ������ Widget ��
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UHouseBuildWidget> HouseWidgetClass;

	// ������ Widget ʵ��
	UPROPERTY()
	UHouseBuildWidget* HouseWidgetInstance;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	//UPROPERTY(meta = (BindWidget))
	//UCanvasPanel* ShowCanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_92;

	UPROPERTY(meta = (BindWidget))
	UScaleBox* ScaleBox_0;

	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox_0;
};
