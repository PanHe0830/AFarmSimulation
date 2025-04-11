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

bool UBuildSubsystem::CheckBuildLocation(const FVector& Location, const FRotator& Rotation, const FVector& Extents) const
{
    UWorld* World = GetWorld();
    if (!World) return false;

    //UE_LOG(LogTemp, Warning, TEXT("Location is : %s  --  Extents is : %s"), *Location.ToString() , *Extents.ToString());

    // 设置碰撞查询参数
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(PreviewBuilding); // 忽略附加到的Actor

    // 执行盒体追踪检测
    FHitResult HitResult;
    bool bHit = World->SweepSingleByChannel(
        HitResult,
        Location,
        Location,
        Rotation.Quaternion(),
        ECC_WorldDynamic, // 使用适合你项目的碰撞通道
        FCollisionShape::MakeBox(Extents),
        CollisionParams
    );

    //if (HitResult.GetActor())
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("%s  hitname = %s") , *HitResult.BoneName.ToString() , *HitResult.GetActor()->GetName());
    //}

    //DrawDebugBox(GetWorld(), Location, Extents , FColor::Blue);

    //if (bHit)
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("bHit is true"));
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("bHit is false"));
    //}

    //UBoxComponent* BoxComponent = PreviewBuilding->GetComponentByClass<UBoxComponent>();
    //if (BoxComponent)
    //{
    //    BoxComponent->OnComponentHit
    //}

    return bHit;
}

bool UBuildSubsystem::BuildSystemSpawnActor(TSubclassOf<AActor> BuildingTemplate, const FVector& Location, const FRotator& Rotation, FActorSpawnParameters SpawnParams)
{
    UWorld* World = GetWorld();
    if (!World) return false;

    PreviewBuilding = World->SpawnActor<AActor>(
        BuildingTemplate,
        Location,
        Rotation,
        SpawnParams
    );

    if (PreviewBuilding)
    {
        TArray<UBoxComponent*> BoxComponents;
        PreviewBuilding->GetComponents<UBoxComponent>(BoxComponents);

        for (auto ite : BoxComponents)
        {
            ite->SetHiddenInGame(false);  // 关键：在游戏中显示
            ite->SetVisibility(true, true);
            ite->SetLineThickness(2.0f);  // 设置线框粗细
        }

        return true;
    }
    return false;
}

void UBuildSubsystem::SetCollisionBoxColor()
{
    TArray<UBoxComponent*> BoxComponents;
    PreviewBuilding->GetComponents<UBoxComponent>(BoxComponents);

    for (int i = 0; i < BoxComponents.Num(); i++)
    {
        FVector BoxExtents = BoxComponents[i]->GetScaledBoxExtent();
        FVector BoxLocation = BoxComponents[i]->GetComponentLocation();
        FRotator BoxRotation = BoxComponents[i]->GetComponentRotation();

        bool bFlag = CheckBuildLocation(BoxLocation, BoxRotation, BoxExtents);
        // 检查位置是否可用
        if (bFlag)
        {
            // 有碰撞 红色
            BoxComponents[i]->ShapeColor = FColor::Red;
            //UE_LOG(LogTemp, Warning, TEXT("有碰撞"));
        }
        else
        {
            // 无碰撞 绿色
            BoxComponents[i]->ShapeColor = FColor::Green;
        }
    }
}

bool UBuildSubsystem::ShowPreview(TSubclassOf<AActor> BuildingTemplate, const FVector& Location, const FRotator& Rotation)
{
    if (!BuildingTemplate) return false; //if (PreviewBuilding) PreviewBuilding->Destroy();

    // 创建预览建筑
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    if (BuildSystemSpawnActor(BuildingTemplate, Location, Rotation, SpawnParams))
    {
        SpawnLocation = Location;
    }
    else
    {
        SpawnLocation = FVector::ZeroVector;
        return false;
    }

    SetCollisionBoxColor();

    return true;
}

void UBuildSubsystem::DestoryCurrentActor()
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

    SpawnLocation = position;

    SetCollisionBoxColor();
}

void UBuildSubsystem::SetCollisionBoxVisibilityHide()
{
    TArray<UBoxComponent*> BoxComponents;
    PreviewBuilding->GetComponents<UBoxComponent>(BoxComponents);

    for (int i = 0; i < BoxComponents.Num(); i++)
    {
        BoxComponents[i]->SetHiddenInGame(true);  
        BoxComponents[i]->SetVisibility(false, true);
        BoxComponents[i]->SetLineThickness(0.0f);  // 设置线框粗细

        BoxComponents[i]->SetComponentTickEnabled(false);

        UE_LOG(LogTemp, Warning, TEXT("Component Valid: %d"), IsValid(BoxComponents[i]));
        UE_LOG(LogTemp, Warning, TEXT("HiddenInGame: %d"), BoxComponents[i]->bHiddenInGame);
    }

    PreviewBuilding->SetActorLocation(SpawnLocation);
}

bool UBuildSubsystem::GetCurrentIsRight()
{
    TArray<UBoxComponent*> BoxComponents;
    PreviewBuilding->GetComponents<UBoxComponent>(BoxComponents);

    for (int i = 0; i < BoxComponents.Num(); i++)
    {
        if (BoxComponents[i]->ShapeColor == FColor::Red)
        {
            return false;
        }
    }

    return true;
}
