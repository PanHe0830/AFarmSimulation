// Fill out your copyright notice in the Description page of Project Settings.


#include "AFarmPlayerController.h"
#include "AFarmSimulation/AFarmSimulationCharacter.h"
#include "AFarmSimulation/Character/TopCharacter.h"
#include "AFarmSimulation/GameMode/FarmGameMode.h"
#include "Kismet/GameplayStatics.h"

AAFarmPlayerController::AAFarmPlayerController()
{
    TopCharacter = nullptr;
    ThridCharacter = nullptr;
    bIsTopViewPort = true;
}

void AAFarmPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // ��ȡĬ�� ACharacter
    ThridCharacter = Cast<AAFarmSimulationCharacter>(GetCharacter()); // ��Ϸ��ʼʱĬ�� Possess ACharacter

    // ���� BP_TopPawn
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATopCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        if (Actor->IsA(ATopCharacter::StaticClass()) && Actor->GetName().Contains("BP_TopCharacter"))
        {
            TopCharacter = Cast<ATopCharacter>(Actor);
            break;
        }
    }

    if (TopCharacter)
    {
        TopCharacter->SetActorHiddenInGame(true);
        TopCharacter->SetActorEnableCollision(false);
    }
}

void AAFarmPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("ChangeViewPort", IE_Pressed, this, &AAFarmPlayerController::SwitchCameraView);
}

void AAFarmPlayerController::SwitchCameraView()
{
    if (!ThridCharacter || !TopCharacter) return;

    if (bIsTopViewPort)
    {
        // �л��� DefaultPawn
        ThridCharacter->SetActorHiddenInGame(true);
        ThridCharacter->SetActorEnableCollision(false);

        TopCharacter->SetActorHiddenInGame(false);
        TopCharacter->SetActorEnableCollision(true);
        TopCharacter->SetActorLocation(ThridCharacter->GetActorLocation());

        Possess(TopCharacter);
        bIsTopViewPort = false;
        UE_LOG(LogTemp , Warning , TEXT("DefaultPawn"));
    }
    else
    {
        // �л��� ACharacter
        TopCharacter->SetActorHiddenInGame(true);
        TopCharacter->SetActorEnableCollision(false);

        ThridCharacter->SetActorHiddenInGame(false);
        ThridCharacter->SetActorEnableCollision(true);

        Possess(ThridCharacter);
        bIsTopViewPort = true;
        UE_LOG(LogTemp, Warning, TEXT("ACharacter"));
    }
}