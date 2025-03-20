// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AFarmPlayerController.generated.h"

/**
 * 
 */
class ATopCharacter;
class AAFarmSimulationCharacter;

UCLASS()
class AFARMSIMULATION_API AAFarmPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAFarmPlayerController();
protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	void SwitchCameraView();

	ATopCharacter* TopCharacter;
	AAFarmSimulationCharacter* ThridCharacter;

	bool bIsTopViewPort;
};
