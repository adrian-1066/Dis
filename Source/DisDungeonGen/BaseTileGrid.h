// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTileGrid.generated.h"

UCLASS()
class DISDUNGEONGEN_API ABaseTileGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseTileGrid();
	virtual void Tick(float DeltaTime) override;

	void SetTileMat(UMaterialInterface* Material);
	UPROPERTY(VisibleAnywhere , Category="Tile Settings")
	UStaticMeshComponent* TileMesh;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Settings")
	UStaticMesh* CubeStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Settings")
	UMaterialInterface* CubeMaterial;*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	



};
