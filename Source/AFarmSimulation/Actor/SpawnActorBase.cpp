// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnActorBase.h"
#include "Components/BoxComponent.h"

// Sets default values
ASpawnActorBase::ASpawnActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Default"));
	SetRootComponent(StaticMesh);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(StaticMesh);
}

// Called when the game starts or when spawned
void ASpawnActorBase::BeginPlay()
{
	Super::BeginPlay();

	//FBox Box = StaticMesh->GetNavigationBounds(); 
	//FTransform ComponentTransform = StaticMesh->GetComponentTransform();
	//FBox WorldBoundingBox = Box.TransformBy(ComponentTransform);
	//
	//FVector Min = WorldBoundingBox.Min;
	//FVector Max = WorldBoundingBox.Max;
	//FVector Center = (Min + Max) * 0.5f;
	//FVector Extent = (Max - Min) * 0.5f;
	//float MaxExtent = FMath::Max3(Extent.X, Extent.Y, Extent.Z);
	//
	//UE_LOG(LogTemp, Warning, TEXT("%s %s %s %s %s"),*ComponentTransform.ToString(), *Min.ToString(), *Max.ToString(), *Center.ToString(), *Extent.ToString());
	//BoxComponent->SetWorldLocation(Center);
	//BoxComponent->SetBoxExtent(Extent);
}

// Called every frame
void ASpawnActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

