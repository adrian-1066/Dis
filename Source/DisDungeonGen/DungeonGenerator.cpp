// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"

#include <SceneExport.h>
//#include "Math/UnrealMathUtility.h"
// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	SetGridUp();
	SetCellStrAndType();
	SetCellhood();
	InitialStrCheck();
	SecondStrCheck();
	//InitialStrCheck();
	SpawnCubes();
	
}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADungeonGenerator::SetGridUp()
{
	TheGrid.SetNum(GridSize);
	for(int i = 0; i < GridSize; i++)
	{
		TheGrid[i].SetNum(GridSize);
		
	}

	for(int x = 0; x < GridSize;x++)
	{
		for(int y = 0; y < GridSize; y++)
		{
			TheGrid[x][y].CellStrength = 0;
			TheGrid[x][y].CellRoomType = 0;
		}
	}
}

void ADungeonGenerator::SetCellStrAndType()
{
	for(int x = 0; x < GridSize; x++)
	{
		for(int y = 0; y < GridSize; y++)
		{
			
			TheGrid[x][y].CellStrength = GetRandomIntInRange(1, 20);
			TheGrid[x][y].CellRoomType = GetRandomIntInRange(1,8);
			TheGrid[x][y].CellPos = FIntVector2(x,y);
			//UE_LOG(LogTemp, Warning, TEXT("the room str is: %d"), TheGrid[x][y].CellStrength);
		}
	}
}

void ADungeonGenerator::SetCellhood()
{
	int TempAmount = 0;
	for(int x = 0; x < GridSize; x++)
	{
		for(int y = 0; y < GridSize; y++)
		{
			TempAmount = 0;
			if(y - 1 >= 0)
			{
				TempAmount++;
				//TheGrid[x][y].Neighbours.Add(TheGrid[x][y-1]);
			}
			
			if(y + 1 <= GridSize-1)
			{
				TempAmount++;
				//TheGrid[x][y].Neighbours.Add(TheGrid[x][y+1]);
			}
		

			if(x - 1 >= 0)
			{
				TempAmount++;
				//TheGrid[x][y].Neighbours.Add(TheGrid[x-1][y]);
			}
			

			if(x + 1 <= GridSize-1)
			{
				TempAmount++;
				//TheGrid[x][y].Neighbours.Add(TheGrid[x+1][y]);
			}
			
			SetNeighborhood(TempAmount,FIntVector2(x,y));
		}
	}
}

void ADungeonGenerator::SetNeighborhood(int Value,  FIntVector2 Cell)
{
	//TheGrid[Cell.X][Cell.Y].Neighbours.SetNum(0);
	//UE_LOG(LogTemp, Warning, TEXT("Array size at x y is : %d %d %d"),Cell.X, Cell.Y, TheGrid[Cell.X][Cell.Y].Neighbours.Num());
	//TheGrid[Cell.X][Cell.Y].Neighbours.SetNum(Value);
	FDungeonCellNeighb TempOne;
	FDungeonCellNeighb TempTwo;
	FDungeonCellNeighb TempThree;
	FDungeonCellNeighb TempFour;
	int index = 0;

	UE_LOG(LogTemp, Warning, TEXT("Value at cell is: %d %d %d"),Value ,Cell.X, Cell.Y);
	if(Cell.X == 0 && Cell.Y == 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Array size at x+1 y+1 is : %d %d %d"),Cell.X+1, Cell.Y+1, TheGrid[Cell.X][Cell.Y].Neighbours.Num());
	}
	
	if(Cell.Y - 1 >= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("y - 1"));
		TempOne.CellPos.X = Cell.X;
		TempOne.CellPos.Y = Cell.Y - 1;
		TempOne.CellStrength = TheGrid[Cell.X][Cell.Y-1].CellStrength;
		TempOne.CellRoomType = TheGrid[Cell.X][Cell.Y-1].CellRoomType;
		//TheGrid[Cell.X][Cell.Y].Neighbours.Insert(TempOne, index);
		TheGrid[Cell.X][Cell.Y].Neighbours.Add(TempOne);
		index++;
		//TheGrid[Cell.X][Cell.Y].Neighbours.Add(TheGrid[Cell.X][Cell.Y-1]);
	}
	if(Cell.Y + 1 <= GridSize-1)
	{
		UE_LOG(LogTemp, Warning, TEXT("y + 1"));
		TempTwo.CellPos.X = Cell.X;
		TempTwo.CellPos.Y = Cell.Y + 1;
		TempTwo.CellStrength = TheGrid[Cell.X][Cell.Y+1].CellStrength;
		TempTwo.CellRoomType = TheGrid[Cell.X][Cell.Y+1].CellRoomType;
		//TheGrid[Cell.X][Cell.Y].Neighbours.Insert(TempTwo, index);
		TheGrid[Cell.X][Cell.Y].Neighbours.Add(TempTwo);
		index++;
		//TheGrid[Cell.X][Cell.Y].Neighbours.Add(TheGrid[Cell.X][Cell.Y+1]);
	}
	if(Cell.X - 1 >= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("x - 1"));
		TempThree.CellPos.X = Cell.X-1;
		TempThree.CellPos.Y = Cell.Y;
		TempThree.CellStrength = TheGrid[Cell.X-1][Cell.Y].CellStrength;
		TempThree.CellRoomType = TheGrid[Cell.X-1][Cell.Y].CellRoomType;
		//TheGrid[Cell.X][Cell.Y].Neighbours.Insert(TempThree, index);
		TheGrid[Cell.X][Cell.Y].Neighbours.Add(TempThree);
		index++;
		//TheGrid[Cell.X][Cell.Y].Neighbours.Add(TheGrid[Cell.X-1][Cell.Y]);
	}
	if(Cell.X + 1 <= GridSize-1)
	{
		UE_LOG(LogTemp, Warning, TEXT("x + 1"));
		TempFour.CellPos.X = Cell.X+1;
		TempFour.CellPos.Y = Cell.Y;
		TempFour.CellStrength = TheGrid[Cell.X+1][Cell.Y].CellStrength;
		TempFour.CellRoomType = TheGrid[Cell.X+1][Cell.Y].CellRoomType;
		//TheGrid[Cell.X][Cell.Y].Neighbours.Insert(TempFour, index);
		TheGrid[Cell.X][Cell.Y].Neighbours.Add(TempFour);
		index++;
		//TheGrid[Cell.X][Cell.Y].Neighbours.Add(TheGrid[Cell.X+1][Cell.Y]);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Array size is now at : %d"), TheGrid[Cell.X][Cell.Y].Neighbours.Num());
}

void ADungeonGenerator::InitialStrCheck()
{
	for(int x = 0; x < GridSize; x++)
	{
		for(int y = 0; y < GridSize; y++)
		{
			for(int i = 0; i < TheGrid[x][y].Neighbours.Num();i++)
			{
				UE_LOG(LogTemp, Warning, TEXT("x y has neighb : %d %d %d"),x,y,TheGrid[x][y].Neighbours.Num());
				int tempX = TheGrid[x][y].Neighbours[i].CellPos.X;
				int tempY = TheGrid[x][y].Neighbours[i].CellPos.Y;
				UE_LOG(LogTemp, Warning, TEXT("str check at x y neighb : %d %d %d"), x, y, i);
				UE_LOG(LogTemp, Warning, TEXT("x y tempX temp Y : %d %d %d %d"), x, y, tempX, tempY);
				//bool IsStrong = StrCheck(x,y,tempX,tempY);

				if(TheGrid[x][y].CellStrength >= TheGrid[x][y].Neighbours[i].CellStrength)
				{
					TheGrid[tempX][tempY].CellRoomType = TheGrid[x][y].CellRoomType;
					TheGrid[tempX][tempY].CellStrength = TheGrid[x][y].CellStrength - 1;
				}
				else
				{
					TheGrid[x][y].CellRoomType = TheGrid[tempX][tempY].CellRoomType;
					TheGrid[x][y].CellStrength = TheGrid[tempX][tempY].CellStrength - 1;
				}
			}
				

			
		}
	}
}

void ADungeonGenerator::SecondStrCheck()
{
	for(int x = GridSize-1; x >= 0; x--)
	{
		for(int y = GridSize-1; y >= 0; y--)
		{
			for(int i = 0; i < TheGrid[x][y].Neighbours.Num();i++)
			{
				UE_LOG(LogTemp, Warning, TEXT("x y has neighb : %d %d %d"),x,y,TheGrid[x][y].Neighbours.Num());
				int tempX = TheGrid[x][y].Neighbours[i].CellPos.X;
				int tempY = TheGrid[x][y].Neighbours[i].CellPos.Y;
				UE_LOG(LogTemp, Warning, TEXT("str check at x y neighb : %d %d %d"), x, y, i);
				UE_LOG(LogTemp, Warning, TEXT("x y tempX temp Y : %d %d %d %d"), x, y, tempX, tempY);
				//bool IsStrong = StrCheck(x,y,tempX,tempY);

				if(TheGrid[x][y].CellStrength >= TheGrid[x][y].Neighbours[i].CellStrength)
				{
					TheGrid[tempX][tempY].CellRoomType = TheGrid[x][y].CellRoomType;
					TheGrid[tempX][tempY].CellStrength = TheGrid[x][y].CellStrength - 1;
				}
				else
				{
					TheGrid[x][y].CellRoomType = TheGrid[tempX][tempY].CellRoomType;
					TheGrid[x][y].CellStrength = TheGrid[tempX][tempY].CellStrength - 1;
				}
			}
				

			
		}
	}
}

bool ADungeonGenerator::StrCheck(int x, int y, int Nx, int Ny)
{
	if(TheGrid[x][y].CellStrength >= TheGrid[Nx][Ny].CellStrength)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void ADungeonGenerator::SpawnCubes()
{
	UWorld* const World = GetWorld();
	if(World)
	{
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		for(int x = 0; x < GridSize; x++)
		{
			for(int y = 0; y < GridSize; y++)
			{
				
				

				FVector SpawnLocation = FVector(x*100,y*100 ,0);
				FRotator SpawnRotation = FRotator(0.0f,0.0f,0.0f);
				if(CubeList[TheGrid[x][y].CellRoomType-1] >= 0)
				{
					World->SpawnActor<AActor>(CubeList[TheGrid[x][y].CellRoomType-1], SpawnLocation, SpawnRotation, SpawnParams);
				}
				//UE_LOG(LogTemp, Warning, TEXT("spawning room : %d %d"), x,y);
			}
		}
	}
}


void ADungeonGenerator::checkIfThereIsASeed()
{
	if(GlobalSeed == 0)
	{
		GlobalSeed = FMath::RandRange(1000000,9999999);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("yeah we got a seed my boi"));
		
	}
}

float ADungeonGenerator::GetRandomFloat()
{
	GlobalSeed = (GlobalSeed * 196314165) + 907633515;
	float temp = FMath::Fmod(static_cast<float>(GlobalSeed), MAX_int32) / MAX_int32;
	if(temp < 0)
	{
		temp += 1.0f;
	}
	temp = FMath::Clamp(temp, 0.0f, 1.0f);  // Ensure the value is within [0, 1)
	//UE_LOG(LogTemp, Warning, TEXT("random float is: %f"), temp);
	return temp;
}	

int ADungeonGenerator::GetRandomIntInRange(int Min, int Max)
{
	float tempRandFloat = GetRandomFloat();
	int temp = FMath::RoundToInt((tempRandFloat * (Max - Min)) + Min);

	if(temp < Min)
	{
		UE_LOG(LogTemp, Warning, TEXT("bruh that number was out of range : %d"), temp);
		//temp = GetRandomIntInRange(Min, Max);
	}
	return temp;
}

