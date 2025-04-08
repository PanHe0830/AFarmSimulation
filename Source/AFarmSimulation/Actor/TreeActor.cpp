// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeActor.h"
#include "Engine/StaticMesh.h"

// Sets default values
ATreeActor::ATreeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tree"));
	//StaticMesh->SetupAttachment(RootComponent);
	SetRootComponent(StaticMesh);
}

// Called when the game starts or when spawned
void ATreeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

