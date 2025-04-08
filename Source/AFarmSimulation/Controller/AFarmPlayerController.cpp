// Fill out your copyright notice in the Description page of Project Settings.


#include "AFarmPlayerController.h"
#include "AFarmSimulation/AFarmSimulationCharacter.h"
#include "AFarmSimulation/Character/TopCharacter.h"
#include "AFarmSimulation/GameMode/FarmGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "AFarmSimulation/Actor/TreeActor.h"
#include "AFarmSimulation/SystemSystem/BuildSubsystem.h"

AAFarmPlayerController::AAFarmPlayerController()
{
    TopCharacter = nullptr;
    ThridCharacter = nullptr;
    bIsTopViewPort = true;
    SetShowMouseCursor(true);
}

void AAFarmPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 获取默认 ACharacter
    ThridCharacter = Cast<AAFarmSimulationCharacter>(GetCharacter()); // 游戏开始时默认 Possess ACharacter

    // 查找 BP_TopPawn
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
        // 切换到 DefaultPawn
        ThridCharacter->SetActorHiddenInGame(true);
        ThridCharacter->SetActorEnableCollision(false);

        TopCharacter->SetActorHiddenInGame(false);
        TopCharacter->SetActorEnableCollision(true);
        TopCharacter->SetActorLocation(ThridCharacter->GetActorLocation());

        Possess(TopCharacter);
        bIsTopViewPort = false;
        //UE_LOG(LogTemp , Warning , TEXT("DefaultPawn"));
    }
    else
    {
        // 切换回 ACharacter
        TopCharacter->SetActorHiddenInGame(true);
        TopCharacter->SetActorEnableCollision(false);

        ThridCharacter->SetActorHiddenInGame(false);
        ThridCharacter->SetActorEnableCollision(true);

        Possess(ThridCharacter);
        bIsTopViewPort = true;
        //UE_LOG(LogTemp, Warning, TEXT("ACharacter"));
    }
}

void AAFarmPlayerController::MouseLeftClicked()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, false, HitResult); // ECC_Visibility 表示检测可见物体

    //if (HitResult.bBlockingHit)
    //{
    //    FVector WorldLocation = HitResult.Location;
    //    //UE_LOG(LogTemp, Warning, TEXT("Clicked at: %s"), *WorldLocation.ToString());
    //
    //    // 可以在这里生成一个调试点（Debug Sphere）来可视化点击位置
    //    DrawDebugSphere(GetWorld(), WorldLocation, 10.f, 12, FColor::Red, false, 2.f);
    //}

    FRotator rotator(0, 0, 0);

    UWorld* World = GetWorld();
    if (World && TreeClass)
    {
        World->SpawnActor<ATreeActor>( 
            TreeClass, 
            HitResult.Location,
            rotator
        );
    }

    InputComponent->RemoveActionBinding("MouseLeftClicked", IE_Pressed);
    InputComponent->RemoveAxisBinding("BuildMouseXY");
}

void AAFarmPlayerController::OpenBuildMode()
{
    InputComponent->BindAction("MouseLeftClicked", IE_Pressed, this, &AAFarmPlayerController::MouseLeftClicked);
    InputComponent->BindAxis("BuildMouseXY", this, &AAFarmPlayerController::MouseXY);
    

    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, false, HitResult); // ECC_Visibility 表示检测可见物体
    FRotator Ratotor(0, 0, 0);
    UBuildSubsystem* Build = GetGameInstance()->GetSubsystem<UBuildSubsystem>();
    if (Build)
    {
        Build->ShowPreview(TreeClass, HitResult.Location, Ratotor);
    }
}

void AAFarmPlayerController::MouseXY(float XY)
{
    //float MouseX;
    //float MouseY;
    //GetMousePosition(MouseX, MouseY);
    //
    //FVector wordLocation;
    //FVector worldDirection;
    //DeprojectMousePositionToWorld(wordLocation, worldDirection);

    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_WorldStatic, false, HitResult); // ECC_Visibility 表示检测可见物体

    UBuildSubsystem* Build = GetGameInstance()->GetSubsystem<UBuildSubsystem>();
    FRotator Ratotor(0, 0, 0);
    if (Build)
    {
        Build->ChangePreviewActorPosition(HitResult.Location);
        UE_LOG(LogTemp, Warning, TEXT("%s"), *HitResult.Location.ToString());
    }
    
}
