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

	CreateBaseVertices();

	SmoothVertices();

	//CreateVertices();

	//CreateTriangles();

	meshComponent->CreateMeshSection(0, vertices, triangles, TArray<FVector>(), UV0, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	meshComponent->SetMaterial(0, material);
}

void ADesertMesh1::CreateBaseVertices()
{
	for (int x = 0; x < xSize; x++)
	{
		for (int y = 0; y < xSize; y++)
		{
			baseVertices.Add(FVector(x * vertexSpacing, y * vertexSpacing, FMath::RandRange(0.0f, 100.0f)));
		}
	}
}

void ADesertMesh1::SmoothVertices()
{
	for (int x = 0; x < (vertices.Num() - 1); x++)
	{
		if ((x + 1) % xSize != 0)
		{
			FVector control1 = baseVertices[x] + FVector(baseVertices[x].X - (vertexSpacing / 2), baseVertices[x].Y, baseVertices[x].Z);
			FVector control2 = baseVertices[x] + FVector(baseVertices[x + 1].X - (vertexSpacing / 2), baseVertices[x].Y, baseVertices[x].Z);
			TArray<FVector> outPoints;
			FVector controlPoints[4];
			controlPoints[0] = vertices[x];
			controlPoints[1] = vertices[x + 1];
			controlPoints[2] = control1;
			controlPoints[3] = control2;
			FVector::EvaluateBezier(controlPoints,smoothPoints, &outPoints);
			vertices.Add(baseVertices[x]);
			for (int y = 0; y < outPoints.Num(); y++)
			{
				vertices.Add(outPoints[y]);
			}
		}
	}
	vertices.Add(baseVertices.Last());
}

void ADesertMesh1::CreateVertices()
{

}

void ADesertMesh1::CreateTriangles()
{
	for (int x = 0; x < (vertices.Num() - xSize - 2); x++)
	{
		if ((x + 1) % xSize != 0)
		{
			triangles.Add(x);
			triangles.Add(x + 1);
			triangles.Add(x + xSize);

			triangles.Add(x + 1);
			triangles.Add(x + 1 + xSize);
			triangles.Add(x + xSize);
		}
	}
}

// Called every frame
void ADesertMesh1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

