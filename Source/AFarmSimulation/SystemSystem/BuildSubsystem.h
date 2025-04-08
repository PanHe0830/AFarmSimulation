// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BuildSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class AFARMSIMULATION_API UBuildSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

    // 检查位置是否可以建造
    UFUNCTION(BlueprintCallable, Category = "Build System")
    bool CanBuildAtLocation(const FVector& Location, const FRotator& Rotation, const FVector& Extents) const;

    // 尝试在指定位置建造
    UFUNCTION(BlueprintCallable, Category = "Build System")
    bool TryBuildAtLocation(AActor* BuildingToPlace, const FVector& Location, const FRotator& Rotation);

private:
    // 执行碰撞检测
    bool CheckBuildLocation(const FVector& Location, const FRotator& Rotation, const FVector& Extents) const;

    UPROPERTY()
    AActor* PreviewBuilding;

public:
    UFUNCTION(BlueprintCallable, Category = "Build System")
    void ShowPreview(TSubclassOf<AActor> BuildingTemplate, const FVector& Location, const FRotator& Rotation);

    UFUNCTION(BlueprintCallable, Category = "Build System")
    void HidePreview();

    UFUNCTION(BlueprintCallable, Category = "Build System")
    void ChangePreviewActorPosition(FVector position);

};
