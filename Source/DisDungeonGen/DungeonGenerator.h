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
	int MinRoomSizeByCell;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dungeon settings")
	int MinRoomSizeWithWall;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dungeon settings")
	int MaxRoomSizeByCell;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dungeon settings")
	TArray<TSubclassOf<AActor>> CubeList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dungeon settings")
	TArray<TSubclassOf<AActor>> WallList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dungeon settings")
	float MinDistBetweenStartAndEnd;

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
	void CreateRooms();
	void AddCellToRoom(int RoomIndex, FDungeonCell* CellToAdd);
	void CreateNewRoom(FDungeonCell* Cell);
	void RoomCleanUp();
	void SmallRoomCleanUp();
	void RoomAdjacentCheck();
	void MinRoomAdjacentCleanUp();
	void ReveseMinRoomCleanUp();
	void RoomMergedAdjChec(int roomIndex);

	void RoomNeighbourUpdate();

	void RoomWallSet();
	void AddRoomsToIgnoreList();
	void SetRoomPos();
	void RoomMerge(int MainRoomIndex, int SmallRoomIndex);
	

	void PickStartAndEndRooms();
	void StartPathFinding();
	int NextRoomInPath(int CurrentRoomID);
	bool IsNextRoomAllowed(int CurrentRoomIndex);
	void CleanWAllsBetweenRooms();
	void SetRoomTypes();
	void SpawnCubes();
	void SpawnWalls();


	bool StrCheck(int x, int y, int Nx, int Ny);
	void checkIfThereIsASeed();
	float GetRandomFloat();
	int GetRandomIntInRange(int Min, int Max);
	void ResetRoomChance();

	TArray<FDungeonRoom> DungeonRooms;
	TArray<int> PathStack;
	TArray<int> RoomsToIgnore;
	TArray<int> RoomsToIgnoreForObj;
	bool PathAtEndPoint;
	int RoomsInGrid;
	int RoomsMerged;
	int StartRoomID;
	int EndRoomID;
	int NumOfRoomsInCurrentPath;
	int MinNumberOfRoomsAllowedInPath;

	int TestNumber;
	int NumOfCellsInDungeon;
	int NumOfFightRooms;
	int NumOfTreasureRooms;
	int NumOfPuzzleRooms;
	int NumOfDeadRooms;

	float CurrentPuzzleChance;
	float CurrentTreasureChance;
	float CurrentFightChance;

	float BasePuzzleChance;
	float BaseTreasureChance;
	float BaseFightChance;
	

};



