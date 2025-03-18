// Fill out your copyright notice in the Description page of Project Settings.

/**
*	这个类是游戏主界面的类，它显示个人信息，个人所持有的资源信息等
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
