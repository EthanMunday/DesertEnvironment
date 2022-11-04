// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertMesh1.h"

// Sets default values
ADesertMesh1::ADesertMesh1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComponent = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
	meshComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ADesertMesh1::BeginPlay()
{
	Super::BeginPlay();

	meshComponent->CreateMeshSection(0, vertices, triangles, TArray<FVector>(), UV0, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	meshComponent->SetMaterial(0, material);
}

// Called every frame
void ADesertMesh1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

