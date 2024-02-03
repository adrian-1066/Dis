// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonStructs.generated.h"

USTRUCT(BlueprintType)
struct DISDUNGEONGEN_API FDungeonCellNeighb
{
 GENERATED_BODY()
 int CellRoomType;
 int CellStrength;
 FIntVector2 CellPos;

 
};

USTRUCT(BlueprintType)
struct DISDUNGEONGEN_API FDungeonCell
{
 GENERATED_BODY()
 int CellRoomType;
 int CellStrength;
 FIntVector2 CellPos;
 int NeighbNum;
 TArray<FDungeonCellNeighb> Neighbours;
	

 
 

};
