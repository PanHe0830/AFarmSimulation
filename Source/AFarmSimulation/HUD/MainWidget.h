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
};
