// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "DesertMesh1.generated.h"

UCLASS()
class DESERTENVIRONMENT_API ADesertMesh1 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADesertMesh1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	int xSize;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	int ySize;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	int smoothPoints;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	float vertexSpacing;

	UPROPERTY(EditAnywhere)
	TArray<FVector> baseVertices;

	UPROPERTY(EditAnywhere,Meta = (MakeEditwidget = true))
	TArray<FVector> vertices;

	UPROPERTY(EditAnywhere)
	TArray <int> triangles;
	
	UPROPERTY(EditAnywhere)
	TArray <FVector2D> UV0;
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* material;

	void CreateBaseVertices();

	void SmoothVertices();

	void CreateVertices();

	void CreateTriangles();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UProceduralMeshComponent* meshComponent;
};
