// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildWidget.h"
#include "Components/Button.h"
#include "AFarmSimulation/Controller/AFarmPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "AFarmSimulation/SystemSystem/BuildSubsystem.h"

bool UBuildWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (ButtonHouse)
	{
		ButtonHouse->OnClicked.AddDynamic(this , &UBuildWidget::CreateHouseMesh);
	}

	return true;
}

void UBuildWidget::CreateHouseMesh()
{
	//UE_LOG(LogTemp, Warning, TEXT("CreateHouseMesh"));
	UWorld* World = GetWorld();
	if (World)
	{
		AAFarmPlayerController* PlayerController = Cast<AAFarmPlayerController>(UGameplayStatics::GetPlayerController(World, 0));
		if (PlayerController)
		{
			PlayerController->OpenBuildMode();
		}
	}
}
