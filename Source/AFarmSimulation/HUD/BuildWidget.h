// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildWidget.generated.h"


class UButton;
/**
 * 
 */
UCLASS()
class AFARMSIMULATION_API UBuildWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void CreateHouseMesh();

public:
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonHouse;
};
