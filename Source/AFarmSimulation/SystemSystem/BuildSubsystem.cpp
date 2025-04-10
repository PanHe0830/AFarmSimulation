// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSubsystem.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void UBuildSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

    SpawnLocation = FVector::ZeroVector;
}

void UBuildSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UBuildSubsystem::CanBuildAtLocation(const FVector& Location, const FRotator& Rotation, const FVector& Extents) const
{
    return !CheckBuildLocation(Location, Rotation, Extents);
}

bool UBuildSubsystem::TryBuildAtLocation(TSubclassOf<AActor> BuildingToPlace, const FVector& Location, const FRotator& Rotation)
{
    if (!BuildingToPlace) return false;

    // 生成建筑
    UWorld* World = GetWorld();
    if (!World) return false;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* NewBuilding = World->SpawnActor<AActor>(
        BuildingToPlace,
        Location,
        Rotation,
        SpawnParams
    );

    // 获取建筑的碰撞体范围
    UBoxComponent* BuildCollision = NewBuilding->FindComponentByClass<UBoxComponent>();
    if (!BuildCollision)
    {
        NewBuilding->Destroy();
        UE_LOG(LogTemp , Warning , TEXT("BuildCollision is nullptr"));
        return false;
    }

    FVector Extents = BuildCollision->GetScaledBoxExtent();

    // 检查位置是否可用
    if (CheckBuildLocation(Location, Rotation, Extents))
    {
        NewBuilding->Destroy();
        return false;
    }

    return NewBuilding != nullptr;
}

bool UBuildSubsystem::CheckBuildLocation(const FVector& Location, const FRotator& Rotation, const FVector& Extents) const
{
    UWorld* World = GetWorld();
    if (!World) return true;

    // 设置碰撞查询参数
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(UGameplayStatics::GetPlayerPawn(World, 0)); // 忽略玩家

    // 执行盒体追踪检测
    FHitResult HitResult;
    bool bHit = World->SweepSingleByChannel(
        HitResult,
        Location,
        Location,
        Rotation.Quaternion(),
        ECC_WorldStatic, // 使用适合你项目的碰撞通道
        FCollisionShape::MakeBox(Extents),
        CollisionParams
    );

    UE_LOG(LogTemp , Warning , TEXT("%s") , bHit? TEXT("TRUE") : TEXT("FALSE"));

    return bHit;
}

bool UBuildSubsystem::ShowPreview(TSubclassOf<AActor> BuildingTemplate, const FVector& Location, const FRotator& Rotation)
{
    if (!BuildingTemplate)
    {
        UE_LOG(LogTemp, Warning, TEXT("BuildingTemplate is nullptr"));
        return false;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("World is not exitst"));
        return false;
    }

    // 如果预览已存在，先销毁
    if (PreviewBuilding)
    {
        UE_LOG(LogTemp, Warning, TEXT("PreviewBuilding is exist"));
        PreviewBuilding->Destroy();
    }

    // 创建预览建筑
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    PreviewBuilding = World->SpawnActor<AActor>(
        BuildingTemplate,
        Location,
        Rotation,
        SpawnParams
    );

    SpawnLocation = Location;

    if (PreviewBuilding == nullptr)
    {
        SpawnLocation = FVector::ZeroVector;
        UE_LOG(LogTemp, Warning, TEXT("PreviewBuilding is failed"));
        return false;
    }
    return true;
}

void UBuildSubsystem::HidePreview()
{
    if (PreviewBuilding)
    {
        PreviewBuilding->Destroy();
        PreviewBuilding = nullptr;
    }
}

void UBuildSubsystem::ChangePreviewActorPosition(FVector position)
{
    if (PreviewBuilding == nullptr) return;
    PreviewBuilding->SetActorLocation(position);
}
