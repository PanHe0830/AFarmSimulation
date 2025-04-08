// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSubsystem.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void UBuildSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UBuildSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UBuildSubsystem::CanBuildAtLocation(const FVector& Location, const FRotator& Rotation, const FVector& Extents) const
{
    return !CheckBuildLocation(Location, Rotation, Extents);
}

bool UBuildSubsystem::TryBuildAtLocation(AActor* BuildingToPlace, const FVector& Location, const FRotator& Rotation)
{
    if (!BuildingToPlace) return false;

    // ��ȡ��������ײ�巶Χ
    UBoxComponent* BuildCollision = BuildingToPlace->FindComponentByClass<UBoxComponent>();
    if (!BuildCollision) return false;

    FVector Extents = BuildCollision->GetScaledBoxExtent();

    // ���λ���Ƿ����
    if (CheckBuildLocation(Location, Rotation, Extents))
    {
        return false;
    }

    // ���ɽ���
    UWorld* World = GetWorld();
    if (!World) return false;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* NewBuilding = World->SpawnActor<AActor>(
        BuildingToPlace->GetClass(),
        Location,
        Rotation,
        SpawnParams
    );

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

    return bHit;
}

void UBuildSubsystem::ShowPreview(TSubclassOf<AActor> BuildingTemplate, const FVector& Location, const FRotator& Rotation)
{
    if (!BuildingTemplate) return;

    UWorld* World = GetWorld();
    if (!World) return;

    // ���Ԥ���Ѵ��ڣ�������
    if (PreviewBuilding)
    {
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
