// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CommonStructs.h"
#include "DungeonGenerator.generated.h"

UCLASS()
class DISDUNGEONGEN_API ADungeonGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonGenerator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dungeon settings")
	int GridSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dungeon settings")
	int GlobalSeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dungeon settings")
	TArray<TSubclassOf<AActor>> CubeList;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:

	
	TArray<TArray<FDungeonCell>> TheGrid;

	void SetGridUp();
	void SetCellStrAndType();
	void SetCellhood();
	void SetNeighborhood(int Value, FIntVector2 Cell);
	void InitialStrCheck();
	void SecondStrCheck();
	void SpawnCubes();


	bool StrCheck(int x, int y, int Nx, int Ny);
	void checkIfThereIsASeed();
	float GetRandomFloat();
	int GetRandomIntInRange(int Min, int Max);
	

};



