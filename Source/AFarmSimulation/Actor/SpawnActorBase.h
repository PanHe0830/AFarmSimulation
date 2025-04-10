// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnActorBase.generated.h"

class UBoxComponent;

UCLASS()
class AFARMSIMULATION_API ASpawnActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = "SpawnActor")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "SpawnActor")
	UBoxComponent* BoxComponent;
};
