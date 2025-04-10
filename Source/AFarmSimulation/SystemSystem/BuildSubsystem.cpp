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

    // ���ɽ���
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

    // ��ȡ��������ײ�巶Χ
    UBoxComponent* BuildCollision = NewBuilding->FindComponentByClass<UBoxComponent>();
    if (!BuildCollision)
    {
        NewBuilding->Destroy();
        UE_LOG(LogTemp , Warning , TEXT("BuildCollision is nullptr"));
        return false;
    }

    FVector Extents = BuildCollision->GetScaledBoxExtent();

    // ���λ���Ƿ����
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

    // ������ײ��ѯ����
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(UGameplayStatics::GetPlayerPawn(World, 0)); // �������

    // ִ�к���׷�ټ��
    FHitResult HitResult;
    bool bHit = World->SweepSingleByChannel(
        HitResult,
        Location,
        Location,
        Rotation.Quaternion(),
        ECC_WorldStatic, // ʹ���ʺ�����Ŀ����ײͨ��
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

    // ���Ԥ���Ѵ��ڣ�������
    if (PreviewBuilding)
    {
        UE_LOG(LogTemp, Warning, TEXT("PreviewBuilding is exist"));
        PreviewBuilding->Destroy();
    }

    // ����Ԥ������
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
