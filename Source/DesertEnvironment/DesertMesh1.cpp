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

	this->SetActorLocation(FVector((- xSize / 2) * vertexSpacing, (- ySize / 2) * vertexSpacing, this->GetActorLocation().Z));

	CreateBaseVertices();

	SmoothVertices();

	SetUV();

	CreateTriangles();

	meshComponent->CreateMeshSection(0, vertices, triangles, TArray<FVector>(), UV0, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	meshComponent->SetMaterial(0, material);
}

void ADesertMesh1::CreateBaseVertices()
{
	for (int x = 0; x < xSize; x++)
	{
		for (int y = 0; y < ySize; y++)
		{
			baseVertices.Add(FVector(x * vertexSpacing, y * vertexSpacing, FMath::RandRange(0.0f, heightVariation)));
		}
	}
}

void ADesertMesh1::SmoothVertices()
{
	int realSmoothPoints = smoothPoints + 2;
	TArray<FVector>* outPoints = new TArray<FVector>;
	for (int x = 0; x < (baseVertices.Num() - 1); x++)
	{
		if ((x + 1) % ySize != 0)
		{
			FVector controlPoints[4];
			FVector middle = (baseVertices[x] + baseVertices[x + 1]) / 2;
			controlPoints[0] = baseVertices[x];
			controlPoints[1] = FVector(middle.X, middle.Y, baseVertices[x].Z);
			controlPoints[2] = FVector(middle.X, middle.Y, baseVertices[x+1].Z);
			controlPoints[3] = baseVertices[x + 1];
			FVector::EvaluateBezier(controlPoints, realSmoothPoints, *outPoints);
			for (int y = 0; y < (outPoints->Num() - 1); y++)
			{
				vertices.Add((*outPoints)[y]);
			}
		}
		else
		{
			vertices.Add(baseVertices[x]);
		}
		outPoints->Empty();
	}

	vertices.Add(baseVertices.Last());

	newXSize = ySize + ((ySize - 1) * smoothPoints);

	int currentSize = vertices.Num();

	for (int x = 0; x < (currentSize - newXSize); x++)
	{
		FVector controlPoints[4];
		FVector middle = (vertices[x] + vertices[x + newXSize]) / 2;
		controlPoints[0] = vertices[x];
		controlPoints[1] = FVector(middle.X, middle.Y, vertices[x].Z);
		controlPoints[2] = FVector(middle.X, middle.Y, vertices[x + newXSize].Z);
		controlPoints[3] = vertices[x + newXSize];


		FVector::EvaluateBezier(controlPoints, realSmoothPoints, *outPoints);
		for (int y = 1; y < (outPoints->Num() - 1); y++)
		{
			vertices.Add((*outPoints)[y]);
		}
		outPoints->Empty();
	}
	newYSize = xSize + ((xSize - 1) * smoothPoints);
	verticalStart = newXSize * xSize;

	delete outPoints;
	outPoints = nullptr;
}

void ADesertMesh1::SetUV()
{
	float xComparison = vertices[verticalStart-1].X - vertices[0].X;
	float yComparison = vertices[verticalStart-1].Y - vertices[0].Y;
	for (int x = 0; x < vertices.Num(); x++)
	{
		float xUV = (vertices[x].X - vertices[0].X) / xComparison * UVScale;
		float yUV = (vertices[x].Y - vertices[0].Y) / xComparison * UVScale;
		UV0.Add(FVector2D(xUV, yUV));
	}
}

void ADesertMesh1::CreateTriangles()
{
	// First Row Triangles
	int baseCheckNum = verticalStart - newXSize;

	for (int x = 0; x < baseCheckNum; x++)
	{
		if ((x + 1) % newXSize != 0)
		{
			triangles.Add(x);
			triangles.Add(x + 1);
			triangles.Add((x * smoothPoints) + verticalStart);

			triangles.Add(x + 1);
			triangles.Add(((x+1) * smoothPoints) + verticalStart);
			triangles.Add((x * smoothPoints) + verticalStart);
		}
	}

	//Middle Row Triangles
	int verticalRow = newXSize;
	for (int x = verticalStart; x < (vertices.Num() - smoothPoints); x += smoothPoints)
	{
		if ((verticalRow + 1) % newXSize != 0)
		{
			for (int y = 0; y < (smoothPoints - 1); y++)
			{
				triangles.Add(x + y);
				triangles.Add(x + y + smoothPoints);
				triangles.Add(x + y + 1);

				triangles.Add(x + y + smoothPoints);
				triangles.Add(x + y + smoothPoints + 1);
				triangles.Add(x + y + 1);
			}
		}
		verticalRow++;
	}

	//Last Row Triangles
	int lastStart = verticalStart - 1 + smoothPoints;
	verticalRow = newXSize;
	for (int x = lastStart; x < (vertices.Num() - smoothPoints); x += smoothPoints)
	{
		if ((verticalRow + 1) % newXSize != 0)
		{
			triangles.Add(x);
			triangles.Add(x + smoothPoints);
			triangles.Add(verticalRow);

			triangles.Add(x + smoothPoints);
			triangles.Add(verticalRow + 1);
			triangles.Add(verticalRow);
		}
		verticalRow++;
	}
}

// Called every frame
void ADesertMesh1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

