// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTileGrid.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
// Sets default values
ABaseTileGrid::ABaseTileGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	RootComponent = TileMesh;

	// Set the static mesh to a cube (assuming the cube mesh is located at "/Engine/BasicShapes/Cube.Cube")
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube"));
	if (CubeAsset.Succeeded())
	{
		TileMesh->SetStaticMesh(CubeAsset.Object);
	}

}

// Called when the game starts or when spawned
void ABaseTileGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseTileGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseTileGrid::SetTileMat(UMaterialInterface* Material)
{
	if(Material && TileMesh)
	{
		TileMesh->SetMaterial(0,Material);
	}
}

